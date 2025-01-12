#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"

void Lock::WriteLock() {

	// ������ �����尡 �����ϰ� �ִٸ� ������ ����
	const uint32 lockThreadId = (m_lockFlag.load() & LockEnum::WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId) {
		++m_iWriteCount;
		return;
	}

	// �ƹ��� ����, �����ϰ� ���� ������, �����ؼ� �������� ����
	const int64 beginTick = ::GetTickCount64();
	const uint32 desired = ((LThreadId << 16) & LockEnum::WRITE_THREAD_MASK);
	while (true) {
		for (uint32 spinCount = 0; spinCount < LockEnum::MAX_SPIN_COUNT; ++spinCount) {
			uint32 expected = LockEnum::EMPTY_FLAG;
			if (m_lockFlag.compare_exchange_strong(OUT expected, desired)) {
				// write lock ��� �ִ� ���¿��� �ٽ� write lock�� ��´ٸ�,
				// ũ���ð� �ƴ϶� write lock�� �÷� ī��Ʈ�� �����ϱ� ����.
				++m_iWriteCount;
				return;
			}
		}

		if (::GetTickCount64() - beginTick >> LockEnum::ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::WriteUnlock() {
	// ReadLock �� Ǯ�� ������ WriteUnlock �Ұ���
	if ((m_lockFlag.load() & LockEnum::READ_COUNT_MASK) != 0)
		CRASH("INVALID_UNLOCK_ORDER")

	const int32 lockCount = --m_iWriteCount;
	if (lockCount == 0)
		m_lockFlag.store(LockEnum::EMPTY_FLAG);
}

void Lock::ReadLock() {
	// ������ �����尡 �����ϰ� �մٸ� ������ ����
	const uint32 lockThreadId = (m_lockFlag.load() & LockEnum::WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId) {
		m_lockFlag.fetch_add(1);
		return;
	}

	// �ƹ��� �����ϰ� ���� ���� �� �����ؼ� ���� ī��Ʈ�� �ø���.
	const int64 beginTick = ::GetTickCount64();
	while (true) {
		for (uint32 spinCount = 0; spinCount < LockEnum::MAX_SPIN_COUNT; ++spinCount) {
			uint32 expected = (m_lockFlag.load() & LockEnum::READ_COUNT_MASK);
			if (m_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
				return;
		}

		if (::GetTickCount64() - beginTick >> LockEnum::ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::ReadUnlock() {
	if ((m_lockFlag.fetch_sub(1) & LockEnum::READ_COUNT_MASK) == 0)
		CRASH("MULTIPLE_UNLOCK");
}

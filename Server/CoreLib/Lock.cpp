#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"

void Lock::WriteLock() {

	// 동일한 스레드가 소유하고 있다면 무조건 성공
	const uint32 lockThreadId = (m_lockFlag.load() & LockEnum::WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId) {
		++m_iWriteCount;
		return;
	}

	// 아무도 소유, 공유하고 있지 않을때, 경합해서 소유권을 얻음
	const int64 beginTick = ::GetTickCount64();
	const uint32 desired = ((LThreadId << 16) & LockEnum::WRITE_THREAD_MASK);
	while (true) {
		for (uint32 spinCount = 0; spinCount < LockEnum::MAX_SPIN_COUNT; ++spinCount) {
			uint32 expected = LockEnum::EMPTY_FLAG;
			if (m_lockFlag.compare_exchange_strong(OUT expected, desired)) {
				// write lock 잡고 있는 상태에서 다시 write lock을 잡는다면,
				// 크래시가 아니라 write lock을 늘려 카운트로 추적하기 위함.
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
	// ReadLock 다 풀기 전에는 WriteUnlock 불가능
	if ((m_lockFlag.load() & LockEnum::READ_COUNT_MASK) != 0)
		CRASH("INVALID_UNLOCK_ORDER")

	const int32 lockCount = --m_iWriteCount;
	if (lockCount == 0)
		m_lockFlag.store(LockEnum::EMPTY_FLAG);
}

void Lock::ReadLock() {
	// 동일한 스레드가 소유하고 잇다면 무조건 성공
	const uint32 lockThreadId = (m_lockFlag.load() & LockEnum::WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId) {
		m_lockFlag.fetch_add(1);
		return;
	}

	// 아무도 소유하고 있지 않을 때 경합해서 공유 카운트를 올린다.
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

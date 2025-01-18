#pragma once
#ifndef __LOCK__
#define __LOCK__

#include "Types.h"



/*
	RW SpinLock
*/

/*-----------------------------------------
| [WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
| W: WriteFlag (Exclusie Lock Owner ThreadId)
| R: ReadFlag (Shared Lock Count)
-----------------------------------------*/

// W -> W (o)
// W -> R (o)
// R -> W (x)
class Lock {
	enum LockEnum: uint32 {
		ACQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};
public:
	void WriteLock(const char*);
	void WriteUnlock(const char*);
	void ReadLock(const char*);
	void ReadUnlock(const char*);

private:
	Atomic<uint32> m_lockFlag = LockEnum::EMPTY_FLAG;
	uint16 m_iWriteCount = 0;
};




// LockGuards

class ReadLockGuard {
public:
	ReadLockGuard(Lock& lock, const char* p_sName)
		: m_lock(lock),
		m_sName(p_sName) {
		m_lock.ReadLock(p_sName);
	}
	~ReadLockGuard() {
		m_lock.ReadUnlock(m_sName);
	}
private:
	Lock& m_lock;
	const char* m_sName;
};

class WriteLockGuard {
public:
	WriteLockGuard(Lock& lock, const char* p_sName)
		: m_lock(lock),
		m_sName(p_sName) {
		m_lock.WriteLock(p_sName);
	}
	~WriteLockGuard() {
		m_lock.WriteUnlock(m_sName);
	}
private:
	Lock& m_lock;
	const char* m_sName;
};



#endif
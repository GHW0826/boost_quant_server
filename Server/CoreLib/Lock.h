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
	void WriteLock();
	void WriteUnlock();
	void ReadLock();
	void ReadUnlock();

private:
	Atomic<uint32> m_lockFlag = LockEnum::EMPTY_FLAG;
	uint16 m_iWriteCount = 0;
};




// LockGuards

class ReadLockGuard {
public:
	ReadLockGuard(Lock& lock) 
		: m_lock(lock) {
		m_lock.ReadLock();
	}
	~ReadLockGuard() {
		m_lock.ReadUnlock();
	}
private:
	Lock& m_lock;
};

class WriteLockGuard {
public:
	WriteLockGuard(Lock& lock)
		: m_lock(lock) {
		m_lock.WriteLock();
	}
	~WriteLockGuard() {
		m_lock.WriteUnlock();
	}
private:
	Lock& m_lock;
};



#endif
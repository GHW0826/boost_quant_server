#pragma once
#ifndef __CORE_MACRO__
#define __CORE_MACRO__

#define OUT

// LOCK
#define USE_MANY_LOCKS(count)	Lock m_locks[count];
#define USE_LOCK				USE_MANY_LOCKS(1)
#define READ_LOCK_IDX(idx)		ReadLockGuard readLockGuard_##idx(m_locks[idx]);
#define READ_LOCK				READ_LOCK_IDX(0)
#define WRITE_LOCK_IDX(idx)		WriteLockGuard writeLockGuard_##idx(m_locks[idx]);
#define WRITE_LOCK				WRITE_LOCK_IDX(0)


// CRASH

#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;						\
}											\

#define ASSERT_CRASH(expr)					\
{											\
	if (!(expr)) {							\
		CRASH("ASSERT_CRASH");				\
		__analysis_assume(expr);			\
	}										\
}											\


#endif
#pragma once
#ifndef __CORE_MACRO__
#define __CORE_MACRO__

#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = xDEADBEEF;						\
}											\

#define ASSERT_CRASH(expr)					\
{											\
	if (!(expr)) {							\
		CRASH("ASSERT_CRASH");				\
		__analysis_assume(expr);			\
	}										\
}											\


#endif
#pragma once
#ifndef _THREAD_MANAGER_
#define _THREAD_MANAGER_

#include <thread>
#include <functional>

class ThreadManager {
public:
	ThreadManager();
	~ThreadManager();

	void Launch(function<void(void)> callback);
	void Join();

	static void InitTLS();
	static void DestroyTLS();
private:
	Mutex m_lock;
	vector<thread>  m_threads;
};

#endif
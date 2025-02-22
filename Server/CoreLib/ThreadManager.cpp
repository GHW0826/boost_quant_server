#include "pch.h"
#include "ThreadManager.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"

ThreadManager::ThreadManager() {
	// Main Thread
	InitTLS();
}

ThreadManager::~ThreadManager() {
	Join();
}

void ThreadManager::Launch(function<void(void)> callback) {
	LockGuard guard(m_lock);

	m_threads.push_back(thread([=]() {
		InitTLS();
		callback();
		DestroyTLS();
	}));
}

void ThreadManager::Join() {
	for (std::thread& t : m_threads)
		if (t.joinable())
			t.join();
	m_threads.clear();
}

void ThreadManager::InitTLS() {
	static Atomic<uint32> SThreadId = 1;
	LThreadId = SThreadId.fetch_add(1);
}

void ThreadManager::DestroyTLS() {
}

#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockProfiler.h"

ThreadManager* GThreadManager = nullptr;
DeadLockProfiler* GDeadLockProfiler = nullptr;

class CoreGlobal {
public:
	CoreGlobal() {
		GThreadManager = new ThreadManager();
		GDeadLockProfiler = new DeadLockProfiler();
	}
	~CoreGlobal() {
		if (GThreadManager) {
			delete GThreadManager;
			GThreadManager = nullptr;
		}
		if (GDeadLockProfiler) {
			delete GDeadLockProfiler;
			GDeadLockProfiler = nullptr;
		}
	}
} CoreGlobal;


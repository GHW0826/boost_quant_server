#pragma once
#ifndef __DEADLOCK_PROFILER__
#define __DEADLOCK_PROFILER__

#include <stack>
#include <map>
#include <vector>

class DeadLockProfiler {

public:
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CheckCycle();

private:
	void Dfs(int32 index);

private:
	unordered_map<const char*, int32> m_NameToId;
	unordered_map<int32, const char*> m_IdToName;
	stack<int32> m_LockStack;
	map<int32, set<int32>> m_LockHistory;

	Mutex m_Lock;

private:
	// 노드가 발견된 순서를 기록하는 배열
	vector<int32> m_DiscoveredOrder;
	// 노드가 발견된 순서
	int32 m_iDiscoveredCount = 0;
	// DFS(i)가 종료 되었는지 여부
	vector<bool> m_Finished;
	vector<int32> m_Parent;
};

#endif
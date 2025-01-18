#include "pch.h"
#include "DeadLockProfiler.h"

void DeadLockProfiler::PushLock(const char* name) {
	LockGuard guard(m_Lock);

	// 아이디를 찾거나 발급.
	int32 iLockId = 0;
	auto findIt = m_NameToId.find(name);
	if (findIt == m_NameToId.end()) {
		iLockId = static_cast<int32>(m_NameToId.size());
		m_NameToId[name] = iLockId;
		m_IdToName[iLockId] = name;
	}
	else
		iLockId = findIt->second;

	// 이미 잡고 있는 락이 있다면
	if (m_LockStack.empty() == false) {
		// 기존에 발견되지 않은 케이스라면 데드락 여부 다시 확인.
		const int32 prevId = m_LockStack.top();
		if (iLockId != prevId) {
			set<int32>& History = m_LockHistory[prevId];
			if (History.find(iLockId) == History.end()) {
				History.insert(iLockId);
				CheckCycle();
			}
		}
	}

	m_LockStack.push(iLockId);
}

void DeadLockProfiler::PopLock(const char* name) {
	LockGuard guard(m_Lock);

	if (m_LockStack.empty())
		CRASH("MULTIPLE_UNLOCK");

	int32 iLockId = m_NameToId[name];
	if (m_LockStack.top() != iLockId)
		CRASH("INVALID_UNLOCK");

	m_LockStack.pop();
}

void DeadLockProfiler::CheckCycle() {

	const int32 iLockCount = static_cast<int32>(m_NameToId.size());
	m_DiscoveredOrder = vector<int32>(iLockCount, -1);
	m_iDiscoveredCount = 0;
	m_Finished = vector<bool>(iLockCount, false);
	m_Parent = vector<int32>(iLockCount, -1);

	for (int32 iLockId = 0; iLockId < iLockCount; ++iLockId)
		Dfs(iLockId);

	// 연산이 끝났으면 정리
	m_DiscoveredOrder.clear();
	m_Finished.clear();
	m_Parent.clear();
}

void DeadLockProfiler::Dfs(int32 here) {

	if (m_DiscoveredOrder[here] != -1)
		return;

	m_DiscoveredOrder[here] = m_iDiscoveredCount++;

	// 모든 인접한 정점을 순회.
	auto findIt = m_LockHistory.find(here);
	if (findIt != m_LockHistory.end()) {
		m_Finished[here] = true;
		return;
	}

	set<int32>& NextSet = findIt->second;
	for (int32 there : NextSet) {
		// 아직 방문한 적이 없다면 방문
		if (m_DiscoveredOrder[there] == -1) {
			m_Parent[there] = here;
			Dfs(there);
			continue;
		}

		// 방문 했다면
		// here가 there 보다 먼저 발견되었다면, there는 here의 후손.
		// (순방향 간선)
		if (m_DiscoveredOrder[here] < m_DiscoveredOrder[there])
			continue;

		// 순방향이 아니고, Dfs(there가 아직 종료하지 않았다면,
		// there는 here의 선조
		// (역방향 간선)
		if (m_Finished[there] == false) {
			// 사이클 상태
			printf("%s -> %s\n", m_IdToName[here], m_IdToName[there]);

			int32 iNow = here;
			while (true) {
				printf("%s -> %s\n", m_IdToName[m_Parent[iNow]], m_IdToName[iNow]);
				iNow = m_Parent[iNow];
				if (iNow == there)
					break;
			}
			CRASH("DEADLOCK DETECTED");
		}
	}

	m_Finished[here] = true;
}

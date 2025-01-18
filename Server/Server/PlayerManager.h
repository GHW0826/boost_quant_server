#pragma once
#ifndef __PLAYER_MANAGER__
#define __PLAYER_MANAGER__

class PlayerManager {
	USE_LOCK;
public:
	void PlayerThenAccount();
	void Lock();
};

extern PlayerManager GPlayerManager;

#endif 

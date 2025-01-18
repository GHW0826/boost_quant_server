#pragma once
#ifndef __ACCOUNT_MANAGER__
#define __ACCOUNT_MANAGER__

class AccountManager {
	USE_LOCK;
public:
	void AccountThenPlayer();
	void Lock();
};

extern AccountManager GAccountManager;


#endif
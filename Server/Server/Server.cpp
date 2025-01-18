// Server.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>


#define _LOCK_CYCLE_EXAMPLE_

#ifdef _LOCK_CYCLE_EXAMPLE_
#include "PlayerManager.h"
#include "AccountManager.h"
#endif 

int main()
{
    std::cout << "Hello World!\n";

#ifdef _LOCK_CYCLE_EXAMPLE_
    GThreadManager->Launch([=] {
        while (true) {
            cout << "PlayerThenAccount" << endl;
            GPlayerManager.PlayerThenAccount();
            this_thread::sleep_for(100ms);
        }
    });

    GThreadManager->Launch([=] {
        while (true) {
            cout << "AccountThenPlayer" << endl;
            GAccountManager.AccountThenPlayer();
            this_thread::sleep_for(100ms);
        }
        });
#endif 
}

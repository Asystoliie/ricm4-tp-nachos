#ifndef USERTHREAD_H
#define USERTHREAD_H

#include "copyright.h"
#include "system.h"
#include "syscall.h"

class UserThread : public Thread {
    public:
        UserThread(const char *name, int f, int a, int callback);
        int func;
        int arg;
        void Fork (); // Make userthread run (*f)(arg)
        void UpdateCallBackRegister (int value); // $31 = value
};

extern int do_UserThreadCreate(int f, int arg, int callback);
extern void StartUserThread(int f);
extern void do_UserThreadExit();
extern int do_UserThreadJoin(int thread_id);

#endif


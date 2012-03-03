#ifndef USERTHREAD_H
#define USERTHREAD_H

#include "copyright.h"
#include "system.h"
#include "syscall.h"

class UserThread : public Thread {
    public:
        UserThread(const char *name, int f, int a);
        int func;
        int arg;
        void Fork (); // Make userthread run (*f)(arg)
};

extern int do_UserThreadCreate(int f, int arg);
extern void StartUserThread(int f);
extern void do_UserThreadExit();

#endif

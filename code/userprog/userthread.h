#ifndef USERTHREAD_H
#define USERTHREAD_H

#include "copyright.h"
#include "system.h"
#include "syscall.h"

typedef struct {
    int f;
    int arg;
} UserThreadArgs;

class UserThread : public Thread {
    public:
        UserThread(const char *debugName, int f, int arg);
        UserThreadArgs *args;
        void StartThread(void);
        ~UserThread();
        int getId () { return (id); }
    private:
        int id;
#ifdef USER_PROGRAM
// ...
#endif
};

extern int do_UserThreadCreate(int f, int arg);
extern void StartUserThread(int f);
extern void do_UserThreadExit();

#endif

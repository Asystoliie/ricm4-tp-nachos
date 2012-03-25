#include "syscall.h"


int main() {
/*    int i = 1;*/
    PutString("Début du père\n...");
    ForkExec("./build/userpages0");
    ForkExec("./build/userpages1");
    ForkExec("./build/userpages2");
/*    for (i = 1; i < 20; i++) {*/
/*        ForkExec("./build/putstring");*/
/*    }*/
    return 0;
}


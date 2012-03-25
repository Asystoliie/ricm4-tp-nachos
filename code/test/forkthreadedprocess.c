#include "syscall.h"


int main() {
    PutString("Debut du pere\n...");
    ForkExec("./build/userpages0");
    ForkExec("./build/userpages1");
    ForkExec("./build/userpages2");
    PutString("Fin du pere\n");
    return 0;
}


//#include "system.h"
#include "syscall.h"


int main() {

    if (ForkExec("./build/putchar")) {
    };
    return 0;
}


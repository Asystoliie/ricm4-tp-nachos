//#include "system.h"
#include "syscall.h"


int main() {

    int result = ForkExec("/home/salem/project/nachos/code/build/putstring");
    if (result < 0) {
        PutString("oups Fils mort né\n");
    };
    PutString("\nFin du père\n");
    Exit(1);
    return 0;
}


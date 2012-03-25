#include "syscall.h"


int main() {
    int i;
    PutString("Début du père\n");
    for (i = 1; i < 16; i++) {
        ForkExec("./build/putstring");
    }
    PutString("Fin du père\n");
    return 0;
}


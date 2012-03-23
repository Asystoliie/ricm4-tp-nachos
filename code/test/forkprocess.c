//#include "system.h"
#include "syscall.h"


int main() {
    PutString("Début du père\n...");
    int result = ForkExec("./build/putstring");
    if (result < 0) {
        PutString("oups Fils 1 mort né\n");
    };
    result = ForkExec("./build/putstring");
    if (result < 0) {
        PutString("oups Fils 2 mort né\n");
    };
    result = ForkExec("./build/putstring");
    if (result < 0) {
        PutString("oups Fils 2 mort né\n");
    };
    result = ForkExec("./build/putstring");
    if (result < 0) {
        PutString("oups Fils 2 mort né\n");
    };
    result = ForkExec("./build/putstring");
    if (result < 0) {
        PutString("oups Fils 2 mort né\n");
    };
    result = ForkExec("./build/putstring");
    if (result < 0) {
        PutString("oups Fils 2 mort né\n");
    };
    result = ForkExec("./build/putstring");
    if (result < 0) {
        PutString("oups Fils 2 mort né\n");
    };
    result = ForkExec("./build/putstring");
    if (result < 0) {
        PutString("oups Fils 2 mort né\n");
    };
    result = ForkExec("./build/putstring");
    if (result < 0) {
        PutString("oups Fils 2 mort né\n");
    };
    result = ForkExec("./build/putstring");
    if (result < 0) {
        PutString("oups Fils 2 mort né\n");
    };
    result = ForkExec("./build/putstring");
    if (result < 0) {
        PutString("oups Fils 2 mort né\n");
    };
    result = ForkExec("./build/userpages0");
    PutString("\nFin du père\n");
    return 0;
}


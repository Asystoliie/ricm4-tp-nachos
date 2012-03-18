//#include "system.h"
#include "syscall.h"

void print(char c) {
    int j = 0;
    for (j=0; j<120; j++) {
        PutChar(c);
    }
/*    UserThreadExit(); // Facultatif*/
}

int main() {
    int t1, t2;
    t1 = UserThreadCreate(print, (void *)'a');
    t2 = UserThreadCreate(print, (void *)'z');
    UserThreadJoin(t1);
    UserThreadJoin(t2);
    PutString("\nFin\n");
    return 0;
}


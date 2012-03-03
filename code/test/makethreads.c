//#include "system.h"
#include "syscall.h"

void print(int i) {
    int j = 0;
    for (j=0; j<12000; j++) {}
        PutInt(i);PutChar('\n');
    UserThreadExit(); // Facultatif
}

int main() {
    PutString("Test de thread : \n");
    int i = 0;
    for (i=0; i<6; i++) {
       UserThreadCreate(print, (void *)i);
    }
    PutString("Fin du main : \n");
    return 0;
}

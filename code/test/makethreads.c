//#include "system.h"
#include "syscall.h"

void print(int i) {
    int j = 0;
    for (j=0; j<120; j++) {}
    PutInt(i);
    UserThreadExit();
}

int main() {
    PutString("Test de thread : \n");
    UserThreadCreate(print, (void *)1);
    int i = 0;
    for (i=0; i<6; i++) {
       UserThreadCreate(print, (void *)i);
    }
    return 0;
}

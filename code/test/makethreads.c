//#include "system.h"
#include "syscall.h"


void print(void * i) {
    int j;
    for (j = 0; j < 20; j++){
        PutInt(j);
    }
    UserThreadExit();
}

int main() {
    PutString("Test de thread : \n");
    UserThreadCreate(print, (void *)20);
    int i = 0;
    for(i=0;i<1000000;i++){}
    return 0;
    PutString("Fin\n");
}


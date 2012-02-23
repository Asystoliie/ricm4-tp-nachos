//#include "system.h"
#include "syscall.h"


void print(void * i) {
    int s = (int) i;
    int j;
    for (j = 0; j < s; j++){
        PutInt(j);PutChar(' ');
    }
}

int main() {
  PutString("Test de thread : \n");
  UserThreadCreate(print, (void *)20);
  PutString("Fin\n");
  return 0;
}


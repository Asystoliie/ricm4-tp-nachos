#include "syscall.h"

int main() {
  PutString("Veuillez saisir un nombre : \n");
  int nombre = GetInt();
  PutString("Nombre +10 = ");PutInt(nombre+10);PutChar('\n');
  PutString("Veuillez saisir une lettre : \n");
  char c = GetChar();
  PutString("Voici la lettre : ");
  PutChar(c);
  PutString("\nVeuillez saisir une phrase (max = 100) : ");
  char buffer[100];
  GetString(buffer,100);
  PutString("\nVoici la phrase : ");
  PutString(buffer);
  PutChar('\n');
  return 0;
}

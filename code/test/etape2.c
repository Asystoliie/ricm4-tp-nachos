#include "syscall.h"

int main() {
  PutString("Veuillez saisir un nombre : \n");
  int value;
  GetInt(&value);
  PutString("++value = ");PutInt(++value);PutChar('\n');
  PutString("Veuillez saisir une lettre : \n");
  char c = GetChar();
  PutString("Voici la lettre : ");
  PutChar(c);
  PutString("\nVeuillez saisir une phrase (max = 100) : ");
  char buffer[100];
  GetString(buffer,100);
  PutString("Voici la phrase : ");
  PutString(buffer);
  PutChar('\n');
  return 0;
}

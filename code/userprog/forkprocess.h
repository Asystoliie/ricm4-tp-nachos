#ifndef FORKPROCESS_H
#define FORKPROCESS_H

#include "copyright.h"
#include "system.h"
#include "syscall.h"

extern int do_ForkExec (char *filename);
extern void StartForkedProcess (int arg);
extern void do_Exit ();

#endif


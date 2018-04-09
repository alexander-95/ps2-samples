#ifndef CONTROLLER_H_
#define CONTROLLER_H_
#include <stdio.h>
#include "libpad.h"
#include <sifrpc.h>

void loadModules();
int waitPadReady(int port, int slot);
int initializePad(int port, int slot);
void padOpen(int port, int slot, int padBuf[256]);
void stabilise(int port, int slot);
#endif

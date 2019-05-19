#ifndef CONTROLLER_H_
#define CONTROLLER_H_
#include <stdio.h>
#include "libpad.h"
#include <sifrpc.h>

struct controller
{
    struct padButtonStatus buttons;
    unsigned int old_pad, new_pad, paddata;
    int port, slot;
};

void loadModules();
int waitPadReady(int port, int slot);
int initializePad(int port, int slot);
void openPad(int port, int slot, int padBuf[256]);
void stabilise(int port, int slot);
struct controller setupController(int* padbuf);
void padUpdate(struct controller* pad);
    
#endif

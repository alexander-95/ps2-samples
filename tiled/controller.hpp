//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <stdio.h>
#include <libpad.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <kernel.h>

class controller
{
public:
    //static int padBuf[256] __attribute__((aligned(64)));
    unsigned int old_pad;
    unsigned int new_pad;
    unsigned int paddata;
    int port;
    int slot;
    struct padButtonStatus buttons;
    
    controller();
    ~controller();
    void loadModules();
    int waitPadReady(int port, int slot);
    int initializePad(int port, int slot);
    void openPad(int port, int slot, int padBuf[256]);
    void stabilise(int port, int slot);
    void read();
    int up();
    int down();
    int left();
    int right();
    int x(u8 singlePress);
    int square(u8 singlePress);
    int circle(u8 singlePress);
    int triangle(u8 singlePress);
};

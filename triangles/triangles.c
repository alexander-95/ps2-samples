//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include "libpad.h"

struct polygon
{
    float x1, y1, x2, y2, x3, y3;
    u64 color;
    char alpha;
    struct polygon* prev;
    struct polygon* next;
};

// it would be nice if this function was non-blocking, ie, if it could be interrupted
// by controller input
void drawRandomTriangle(GSGLOBAL* gsGlobal)
{
    struct polygon p1;
    p1.alpha = 127;
    p1.x1 = rand() % 640;
    p1.x2 = rand() % 640;
    p1.x3 = rand() % 640;
    p1.y1 = rand() % 512;
    p1.y2 = rand() % 512;
    p1.y3 = rand() % 512;
    while(p1.alpha > 64)
    {
        p1.alpha--;
        p1.color = GS_SETREG_RGBAQ(0x00, 0x00, 0xFF, p1.alpha, 0x00);
        gsKit_prim_triangle(gsGlobal, p1.x1, p1.y1, p1.x2, p1.y2, p1.x3, p1.y3, 0, p1.color);
        gsKit_queue_exec(gsGlobal);
        gsKit_sync_flip(gsGlobal);
    }
    p1.alpha = 127;
}

static int padBuf[256] __attribute__((aligned(64)));
static char actAlign[6];
static int actuators;
static void loadModules(void)
{
    int ret;
    ret = SifLoadModule("rom0:SIO2MAN", 0, NULL);
    if (ret < 0)
    {
        printf("sifLoadModule sio failed: %d\n", ret);
        SleepThread();
    }

    ret = SifLoadModule("rom0:PADMAN", 0, NULL);
    if (ret < 0)
    {
        printf("sifLoadModule pad failed: %d\n", ret);
        SleepThread();
    }
}

static int waitPadReady(int port, int slot)
{
    int state;
    int lastState;
    char stateString[16];

    state = padGetState(port, slot);
    lastState = -1;
    while((state != PAD_STATE_STABLE) && (state != PAD_STATE_FINDCTP1))
    {
        if (state != lastState)
        {
            padStateInt2String(state, stateString);
            printf("Please wait, pad(%d,%d) is in state %s\n",
                   port, slot, stateString);
        }
        lastState = state;
        state=padGetState(port, slot);
    }
    // Were the pad ever 'out of sync'?
    if (lastState != -1) printf("Pad OK!\n");
    return 0;
}

static int initializePad(int port, int slot)
{
    waitPadReady(port, slot);
    actuators = padInfoAct(port, slot, -1, 0);
    printf("# of actuators: %d\n",actuators);

    if (actuators != 0)
    {
        actAlign[0] = 0;   // Enable small engine
        actAlign[1] = 1;   // Enable big engine
        actAlign[2] = 0xff;
        actAlign[3] = 0xff;
        actAlign[4] = 0xff;
        actAlign[5] = 0xff;
        waitPadReady(port, slot);
        printf("padSetActAlign: %d\n", padSetActAlign(port, slot, actAlign));
    }
    else printf("Did not find any actuators.\n");
    waitPadReady(port, slot);
    return 1;
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    GSGLOBAL* gsGlobal = gsKit_init_global();

    gsGlobal->PrimAlphaEnable = GS_SETTING_ON;

    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

    //Initialize the DMAC
    dmaKit_chan_init(DMA_CHANNEL_GIF);
    gsKit_init_screen(gsGlobal);

    u64 White = GS_SETREG_RGBAQ(0xFF,0xFF,0xFF,0x00,0x00);// set color
    gsKit_mode_switch(gsGlobal, GS_PERSISTENT);
    gsKit_clear(gsGlobal, White);
    gsKit_mode_switch(gsGlobal, GS_ONESHOT);
    gsKit_queue_exec(gsGlobal);
    gsKit_sync_flip(gsGlobal);

    //controller setup
    unsigned int old_pad = 0;
    unsigned int new_pad, paddata;
    int i, ret, port, slot;
    struct padButtonStatus buttons;
    SifInitRpc(0);
    loadModules();
    padInit(0);
    if((ret = padPortOpen(port, slot, padBuf)) == 0)
    {
        printf("padOpenPort failed: %d\n", ret);
        SleepThread();
    }
    if(!initializePad(port, slot))
    {
        printf("pad initalization failed!\n");
        SleepThread();
    }

    while(1)
    {
        i=0;
        ret=padGetState(port, slot);
        while((ret != PAD_STATE_STABLE) && (ret != PAD_STATE_FINDCTP1))
        {
            if(ret==PAD_STATE_DISCONN) printf("Pad(%d, %d) is disconnected\n", port, slot);
            ret = padGetState(port, slot);
        }
        if(i==1) printf("Pad: OK!\n");
        ret = padRead(port, slot, &buttons);
        if(ret != 0)
        {
            paddata = 0xffff ^ buttons.btns;

            new_pad = paddata & ~old_pad;
            old_pad = paddata;

            if(new_pad & PAD_CROSS) drawRandomTriangle(gsGlobal);
        }
    }
    return 0;
}

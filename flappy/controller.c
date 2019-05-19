#include "controller.h"
#include <loadfile.h>
#include <kernel.h>

void loadModules()
{
    int ret=0;
    ret = SifLoadModule("rom:SIO2MAN", 0, NULL);
    if (ret < 0)ret = SifLoadModule("host:SIO2MAN", 0, NULL);
    if (ret < 0)
    {
        printf("sifLoadModule sio failed: %d\n", ret);
        SleepThread();
    }
   
    ret = SifLoadModule("rom:PADMAN", 0, NULL);
    if(ret < 0)ret = SifLoadModule("host:PADMAN", 0, NULL);
    if (ret < 0)
    {
        printf("sifLoadModule pad failed: %d\n", ret);
        
        SleepThread();
    }
    
}

void openPad(int port, int slot, int padBuf[256])
{
  int ret;
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
}

void stabilise(int port, int slot)
{
  int ret;
  ret=padGetState(port, slot);
  while((ret != PAD_STATE_STABLE) && (ret != PAD_STATE_FINDCTP1))
  {
      if(ret==PAD_STATE_DISCONN) printf("Pad(%d, %d) is disconnected\n", port, slot);
      ret = padGetState(port, slot);
  }
  //printf("Pad: OK!\n");
}

int waitPadReady(int port, int slot)
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
    if (lastState != -1)
    {
        printf("Pad OK!\n");
        }
    return 0;
}

int initializePad(int port, int slot)
{
    static int actuators;
    static char actAlign[6];

    int ret;
    int modes;
    int i;

    waitPadReady(port, slot);

    // How many different modes can this device operate in?
    // i.e. get # entrys in the modetable
    modes = padInfoMode(port, slot, PAD_MODETABLE, -1);
    printf("The device has %d modes\n", modes);

    if (modes > 0) {
        printf("( ");
        for (i = 0; i < modes; i++) {
            printf("%d ", padInfoMode(port, slot, PAD_MODETABLE, i));
        }
        printf(")");
    }

    printf("It is currently using mode %d\n",
           padInfoMode(port, slot, PAD_MODECURID, 0));

    // If modes == 0, this is not a Dual shock controller
    // (it has no actuator engines)
    if (modes == 0) {
        printf("This is a digital controller?\n");
        return 1;
    }

    // Verify that the controller has a DUAL SHOCK mode
    i = 0;
    do {
        if (padInfoMode(port, slot, PAD_MODETABLE, i) == PAD_TYPE_DUALSHOCK)
            break;
        i++;
    } while (i < modes);
    if (i >= modes) {
        printf("This is no Dual Shock controller\n");
        return 1;
    }

    // If ExId != 0x0 => This controller has actuator engines
    // This check should always pass if the Dual Shock test above passed
    ret = padInfoMode(port, slot, PAD_MODECUREXID, 0);
    if (ret == 0) {
        printf("This is no Dual Shock controller??\n");
        return 1;
    }

    printf("Enabling dual shock functions\n");

    // When using MMODE_LOCK, user cant change mode with Select button
    padSetMainMode(port, slot, PAD_MMODE_DUALSHOCK, PAD_MMODE_LOCK);

    waitPadReady(port, slot);
    printf("infoPressMode: %d\n", padInfoPressMode(port, slot));

    waitPadReady(port, slot);
    printf("enterPressMode: %d\n", padEnterPressMode(port, slot));
    
    
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

struct controller setupController(int* padBuf)
{
    struct controller pad;
    pad.old_pad = 0;
    pad.port = 0, pad.slot = 0;
    SifInitRpc(0);
    loadModules();
    padInit(0);
    openPad(pad.port,pad.slot,padBuf);
    stabilise(pad.port,pad.slot);
    return pad;
}

void padUpdate(struct controller* pad)
{
    if(padRead(pad->port, pad->slot, &pad->buttons) != 0)
    {
        pad->paddata = 0xffff ^ pad->buttons.btns;
        pad->new_pad = pad->paddata & ~pad->old_pad;
        pad->old_pad = pad->paddata;
    }
}

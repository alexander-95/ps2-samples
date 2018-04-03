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
#include <libpad.h>
#include "controller.h"

struct polygon
{
    float x1, y1, x2, y2, x3, y3;
    u64 color;
    char alpha;
    struct polygon* prev;
    struct polygon* next;
};

struct pipe
{
  int x, y;
  int d;
  struct pipe* next;
  struct pipe* prev;
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

void drawPipes(GSGLOBAL* gsGlobal, struct pipe* head)
{
  struct pipe* curr = head;
  u64 green  = GS_SETREG_RGBAQ(0x00,0xFF,0x00,0x00,0x00);
  int bottom = 400;
  while(curr!=NULL)
  {
    gsKit_prim_quad(gsGlobal,
		    curr->x, curr->y,
		    curr->x, bottom,
		    curr->x+curr->d, curr->y,
		    curr->x+curr->d, bottom, 6, green);
    curr = curr->next;
  }
}

void movePipes(struct pipe* head, int val)
{
  struct pipe* curr = head;
  struct pipe* tail = head;
  u64 green  = GS_SETREG_RGBAQ(0x00,0xFF,0x00,0x00,0x00);
  int bottom = 400;
  while(curr!=NULL)
  {
    curr->x -= val;
    tail = curr;
    curr = curr->next;
  }
  if(head->x < -50)
  {
    head->x = 640;
    tail->next = head;
    tail->next->next = NULL;
    tail = tail->next;
    head = head->next;
  }
}

static int padBuf[256] __attribute__((aligned(64)));

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
    int i, ret, port=0, slot=0;
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

    u64 red    = GS_SETREG_RGBAQ(0x00,0xFF,0x00,0x00,0x00);
    u64 green  = GS_SETREG_RGBAQ(0x00,0xFF,0x00,0x00,0x00);
    u64 blue   = GS_SETREG_RGBAQ(0x00,0x00,0xFF,0x00,0x00);
    u64 yellow = GS_SETREG_RGBAQ(0xFF,0xFF,0x00,0x00,0x00);

    //background dimensions
    int width = 640, height = 448;

    //platform dimensions
    int top = 380, bottom = 400;

    // create a linked list of pipes
    struct pipe* head = malloc(sizeof(struct pipe));
    head->x = 200, head->y = 200, head->d = 50;
    struct pipe* curr = head;
    int length = 3;
    for(i=1;i<length;i++)
    {
      struct pipe* p = malloc(sizeof(struct pipe));
      curr->next = p;
      p->prev = curr;
      curr = curr->next;
      curr->x = curr->prev->x + 100, curr->y = 200, curr->d = 50;
    }
    curr->next = NULL;
    
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

            if(new_pad & PAD_CROSS)printf("button pressed\n");
        }
	//draw background
	gsKit_prim_quad(gsGlobal,
			0, 0,
			0, height,
			width, 0,
			width, height, 5, blue);
	
	//draw platform
	gsKit_prim_quad(gsGlobal,
			0, bottom,
			0, height,
			width, bottom,
			width, height, 6, yellow);
	gsKit_prim_quad(gsGlobal,
			0, top,
			0, bottom,
			width, top,
			width, bottom, 6, green);

	//draw pipe
	drawPipes(gsGlobal, head);
	movePipes(head, 2);
	//if(head->x < head->d*-1)head->x = width;//should also reset y to some random value
	
	gsKit_sync_flip(gsGlobal);
	gsKit_queue_exec(gsGlobal);
    }
    return 0;
}

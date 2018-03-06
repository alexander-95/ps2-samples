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

enum directions {west, north, east, south};

struct link
{
    char direction; // direction to next link(from head to tail)
    struct link* next;
    struct link* prev;
};

static struct snake
{
    unsigned int x,y; // x,y co-ordinates of the head
    struct link* head;
    struct link* tail;
    char direction;// direction the snake is facing (the opposite of the head link direction)
    unsigned int length;
};

void drawPixel(GSGLOBAL* gsGlobal, unsigned int x, unsigned int y, unsigned int scale)
{
    u64 black = GS_SETREG_RGBAQ(0x00,0x00,0x00,0x00,0x00);// set color
    gsKit_prim_quad(gsGlobal, scale*(x+1), scale*y, x*scale, y*scale, scale*(x+1), scale*(y+1), x*scale, scale*(y+1), 0, black);
}

struct snake* spawnSnake(unsigned int width, unsigned int height)
{
    struct snake* s = malloc(sizeof(struct snake));
    s->x = width / 2;
    s->y = height / 2;
    s->direction = east;
    struct link* l = malloc(sizeof(struct link));
    s->head = l;
    s->head->direction = west;
    struct link* curr = s->head;
    s->length = 3; // starting length for the snake
    int i;
    for(i = 0; i < s->length; i++)
    {
        struct link* l = malloc(sizeof(struct link));
        curr->next = l;
        l->prev = curr;
        l->direction = west;
        s->tail = curr;
        curr = curr->next;
    }
    return s;
}

void respawnSnake(struct snake* s, unsigned int width, unsigned int height)
{
    // clean up the old snake first
    struct link* curr = s->head;
    while(curr->next != NULL)
    {
        struct link* next = curr->next;
        free(curr);
        curr = next;
    }
    
    s->x = width / 2;
    s->y = height / 2;
    s->direction = east;
    struct link* l = malloc(sizeof(struct link));
    s->head = l;
    s->head->direction = west;
    curr = s->head;
    s->length = 3; // starting length for the snake
    unsigned int i;
    for(i = 0; i < s->length; i++)
    {
        struct link* l = malloc(sizeof(struct link));
        curr->next = l;
        l->prev = curr;
        l->direction = west;
        s->tail = curr;
        curr = curr->next;
    }
}

void endGame(struct snake* s)
{
    printf("game over!\nscore:%d\n", s->length - 3);
    respawnSnake(s, 32, 24);
}

void drawSnake(GSGLOBAL* gsGlobal, struct snake* s, unsigned int scale, unsigned int width, unsigned int height)
{
    struct link* curr = s->head;
    unsigned int x = s->x;
    unsigned int y = s->y;
    if(x >= width || y >= height)
    {
        endGame(s);
    }
    while(curr!=NULL)
    {   
        drawPixel(gsGlobal, x, y, scale);
        if(curr->direction == west) x--;
        else if(curr->direction == north) y--;
        else if(curr->direction == east) x++;
        else if(curr->direction == south) y++;
        curr = curr->next;

        //check if this link was eaten
        if(x == s->x && y == s->y)
        {
            endGame(s);
        }
    }
    gsKit_queue_exec(gsGlobal);
    gsKit_sync_flip(gsGlobal);
}

void growSnake(struct snake* s)
{
    struct link* l = malloc(sizeof(struct link));
    l->prev = s->tail;
    l->next = NULL;
    s->tail->next = l;
    s->tail = l;
    s->length++;
}

void moveSnake(struct snake* s)
{
    s->head->prev = s->tail;
    s->tail->next = s->head;
    s->tail = s->tail->prev;
    s->head = s->head->prev;
    s->head->prev = NULL;
    s->tail->next = NULL;
    if(s->direction == west){s->x--;s->head->direction = east;}
    else if(s->direction == north){s->y--;s->head->direction = south;}
    else if(s->direction == east){s->x++;s->head->direction = west;}
    else if(s->direction == south){s->y++;s->head->direction = north;}
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

    // Initialize the DMAC
    dmaKit_chan_init(DMA_CHANNEL_GIF);
    gsKit_init_screen(gsGlobal);

    // draw the background
    u64 white = GS_SETREG_RGBAQ(0xFF,0xFF,0xFF,0x00,0x00);// set color
    unsigned int width = 32, height = 24; // width and height in texels
    unsigned int scale = 16; // width of each texel in pixels
    gsKit_mode_switch(gsGlobal, GS_PERSISTENT);
    gsKit_prim_quad(gsGlobal, width*scale, 0, 0, 0, width*scale, height*scale, 0, height*scale, 0, white);
    gsKit_queue_exec(gsGlobal);
    gsKit_sync_flip(gsGlobal);
    gsKit_mode_switch(gsGlobal, GS_ONESHOT);
    
    // controller setup
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

    // spawn a snake
    struct snake* s = spawnSnake(width, height);

    // spawn food
    unsigned int x = rand() % width, y = rand() % height;
    
    while(1)// event loop
    {
        drawPixel(gsGlobal, x, y, scale);
        ret=padGetState(port, slot);
        while((ret != PAD_STATE_STABLE) && (ret != PAD_STATE_FINDCTP1))
        {
            if(ret==PAD_STATE_DISCONN) printf("Pad(%d, %d) is disconnected\n", port, slot);
            ret = padGetState(port, slot);
        }
        ret = padRead(port, slot, &buttons);
        if(ret != 0)
        {
            paddata = 0xffff ^ buttons.btns;
            new_pad = paddata & ~old_pad;
            old_pad = paddata;

            // react to controller input
            if(new_pad & PAD_LEFT && s->direction%2)s->direction = west;
            else if(new_pad & PAD_UP && s->direction%2==0)s->direction = north;
            else if(new_pad & PAD_RIGHT && s->direction%2)s->direction = east;
            else if(new_pad & PAD_DOWN && s->direction%2==0)s->direction = south;
            
            moveSnake(s);
            drawSnake(gsGlobal, s, scale, width, height);
            if(s->x == x && s->y == y)
            {
                printf("eat food\nscore:%d\n", s->length - 2);
                growSnake(s);
                x = rand() % width, y = rand() % height;
            }
        }
        // TODO: this sleep needs to be non-blocking to allow controller input
        sleep(0.01);
        
    }
    return 0;
}
 

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
#include <dmaKit.h>
#include <gsToolkit.h>
#include <malloc.h>
#include "controller.h"

struct pipe
{
    int x, y;
    int d;
    struct pipe* next;
    struct pipe* prev;
};

struct pipeList
{
    struct pipe* head;
    struct pipe* tail;
    int length;
    int gap;
};

struct bird
{
    int x, y;
    float theta;
    float vy;
};

int collision(struct bird* b, struct pipeList* pipes)
{
    struct pipe* curr = pipes->head;
    while(curr!=NULL)
    {
        char xCollision = b->x+30 > curr->x && b->x < curr->x+curr->d;
        char yCollision = b->y - 24 > curr->y+50 || b->y < curr->y-50;
        if(xCollision &&  yCollision)return 1;
        curr = curr->next;
    }
    return 0;
}

void drawBird(GSGLOBAL* gsGlobal, struct bird* b, GSTEXTURE* tex)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    gsKit_prim_quad_texture(gsGlobal, tex,
                            b->x, b->y,       // x1, y1
                            0.0f, 0.0f,       // u1, v1
                            
                            b->x, b->y+24,    // x2, y2
                            0.0f, 12.0f,      // u2, v2
                            
                            b->x+34, b->y,    // x3, y3
                            17.0f, 0.0f,      // u3, v3
                            
                            b->x+34, b->y+24, // x4, y4
                            17.0f, 12.0f,     // u4, v4
                            3, TexCol);
}

int birdTouchingGround(struct bird* b, int ground)
{
    if(b->y > ground)
    {
        return 1;
    }
    return 0;
}

void drawPipes(GSGLOBAL* gsGlobal, struct pipeList* ps, GSTEXTURE* lower)
{
    struct pipe* curr = ps->head;
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    while(curr!=NULL)
    {
        //upper pipe
        gsKit_prim_quad_texture(gsGlobal, lower,
                                curr->x, curr->y-562,         // x1, y1
                                26.0f, 0.0f,                  // u1, v1
                                
                                curr->x, curr->y-50,          // x2, y2
                                26.0f, 256.0f,                // u2, v2
                                
                                curr->x+curr->d, curr->y-562, // x3, y3
                                52.0f, 0.0f,                  // u3, v3
                        
                                curr->x+curr->d, curr->y-50,  // x4, y4
                                52.0f, 256.0f,                // u4, v4
                                1, TexCol);

        gsKit_prim_quad_texture(gsGlobal, lower,
                                curr->x, curr->y+50,          // x1, y1
                                0.0f, 0.0f,                   // u1, v1
                                    
                                curr->x, curr->y+562,         // x2, y2
                                0.0f, 256.0f,                 // u2, v2
                                    
                                curr->x+curr->d, curr->y+50,  // x3, y3
                                26.0f, 0.0f,                  // u3, v3
                            
                                curr->x+curr->d, curr->y+562, // x4, y4
                                26.0f, 256.0f,                // u4, v4
                                1, TexCol);
        curr = curr->next;
    }
}

void movePipes(struct pipeList* ps, int val, struct bird* b, int* score)
{
    struct pipe* curr = ps->head;
    int i;
    for(i=0;i<ps->length;i++)
    {
        if(b->x >= curr->x && b->x < curr->x+val)
        {
            (*score)++;
            //printf("score:%d\n",*score);
            //FILE* f;
            //f = fopen("mass:log.txt", "a");
            //fprintf(f, "score:%d\n", *score);
            //fclose(f);
        }
        curr->x -= val;
        ps->tail = curr;
        curr = curr->next;
    }
    if(ps->head->x < -52)
    {
        ps->head->x = ps->tail->x+ps->gap;
        ps->head->y = rand() % 300 + 50;
        ps->tail->next = ps->head;
        ps->head->prev = ps->tail;
        ps->head = ps->head->next;
        ps->tail = ps->head->prev;
        ps->tail->next = NULL;
        ps->head->prev = NULL;
    }
}

void printScore(GSGLOBAL* gsGlobal, int score, GSTEXTURE* sprites)
{
    int length, curr;
    int temp = score;
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    
    for(length=0; temp>0; temp/=10)length++;
    if(score==0)length=1;
    printf("score:%d ",score);
    for(;length>0;length--)
    {
        curr = score%10;
        printf("%d",curr);
        score/=10;
    }
    printf("\n");
    /* This if statement should be swapped out for a for loop.
     * Need to print the letters backwards, decreasing the x coordinate accordingly
     * To position the text correctly, the length of the number should be used to 
     * center the text.
     * Text should be at a fixed, predetermined height (y value)
     */
    if(length == 1)
    {
        curr = score;
        //printf("%d\n",*score);
        gsKit_prim_quad_texture(gsGlobal, sprites,
                                0.0f, 0.0f,             // x1, y1
                                52.0f+(12*curr), 0.0f,  // u1, v1
                                    
                                0.0f, 18.0f,            // x2, y2
                                52.0f+(12*curr), 18.0f, // u2, v2
                                    
                                12.0f, 0.0f,            // x3, y3
                                64.0f+(12*curr), 0.0f,  // u3, v3
                            
                                12.0f, 18.0f,           // x4, y4
                                64.0f+(12*curr), 18.0f, // u4, v4
                                1, TexCol);
    }
}

static int padBuf[256] __attribute__((aligned(64)));

int main(int argc, char* argv[])
{
    srand(time(NULL));

    //controller setup
    unsigned int old_pad = 0;
    unsigned int new_pad, paddata;
    int i, port=0, slot=0;
    struct padButtonStatus buttons;
    SifInitRpc(0);
    loadModules();
    padInit(0);
    openPad(port,slot,padBuf);

    //platform dimensions
    int top = 380;
    struct pipeList* pipes = malloc(sizeof(struct pipeList));
    pipes->gap = 200;
    struct pipe* curr = malloc(sizeof(struct pipe));
    pipes->head = curr;
    pipes->length = 4;
    for(i=0;i<pipes->length;i++)
    {
        struct pipe* p = malloc(sizeof(struct pipe));
        curr->next = p;
        p->prev = curr;
        curr->x = 640+pipes->gap*(i), curr->y = rand() % 300 + 50, curr->d = 52;
        curr = curr->next;
    }

    struct bird* b = malloc(sizeof(struct bird));
    b->x = 200;
    b->y = 200;
    b->vy = 0;

    int gravity = 0, collided = 0, score = 0;

    GSGLOBAL* gsGlobal = gsKit_init_global();
    //gsGlobal->Mode = GS_MODE_PAL;
    //gsGlobal->Width=640;
    //gsGlobal->Height=512;
    //gsGlobal->ZBuffering = GS_SETTING_OFF;

    
    GSTEXTURE bg;
    bg.Width=320;
    bg.Height=256;
    bg.PSM = GS_PSM_CT24;

    GSTEXTURE ground;
    ground.Width = 320;
    ground.Height = 56;
    ground.PSM = GS_PSM_CT24;

    GSTEXTURE brd;
    brd.Width = 17;
    brd.Height = 12;
    brd.PSM = GS_PSM_CT24;

    GSTEXTURE lowerPipe;
    lowerPipe.Width = 320;
    lowerPipe.Height = 256;
    lowerPipe.PSM = GS_PSM_CT24;

    GSTEXTURE upperPipe;
    upperPipe.Width = 26;
    upperPipe.Height = 256;
    upperPipe.PSM = GS_PSM_CT24;
    
    u64 White = GS_SETREG_RGBAQ(0xFF,0xFF,0xFF,0x00,0x00);// set color
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);

    gsGlobal->ZBuffering = GS_SETTING_OFF;
    
    //gsGlobal->PSM = GS_PSM_CT24;
    //gsGlobal->PSMZ = GS_PSMZ_16S;

    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

    //Initialize the DMAC
    dmaKit_chan_init(DMA_CHANNEL_GIF);
    gsKit_init_screen(gsGlobal);
    gsKit_mode_switch(gsGlobal, GS_PERSISTENT);
    gsKit_clear(gsGlobal, White);
    gsKit_set_clamp(gsGlobal, GS_CMODE_CLAMP);
    
    gsKit_texture_bmp(gsGlobal, &bg, "mass:bg2.bmp"); // should be tiled
    gsKit_texture_bmp(gsGlobal, &ground, "mass:ground.bmp"); // should be tiled
    //gsKit_texture_bmp(gsGlobal, &brd, "mass:brd.bmp");
    gsKit_texture_bmp(gsGlobal, &lowerPipe, "mass:lowerPipe.bmp"); // rename to spritesheet
    //gsKit_texture_bmp(gsGlobal, &upperPipe, "mass:lowerPipe.bmp");
    //gsKit_texture_bmp(gsGlobal, &sprites, "mass:bigtex.bmp");
    
    gsKit_prim_quad_texture(gsGlobal, &bg,
                            0.0f, 0.0f,     // x1, y1
                            0.0f, 0.0f,     // u1, v1
                                    
                            0.0f, 512.0f,   // x2, y2
                            0.0f, 256.0f,   // u2, v2
                                    
                            640.0f, 0.0f,   // x3, y3
                            320.0f, 0.0f,   // u3, v3
                            
                            640.0f, 512.0f, // x4, y4
                            320.0f, 256.0f, // u4, v4
                            0,TexCol);

    gsKit_mode_switch(gsGlobal, GS_ONESHOT);
    while(1)
    {
	stabilise(port,slot);
        if(padRead(port, slot, &buttons) != 0)
        {
            paddata = 0xffff ^ buttons.btns;

            new_pad = paddata & ~old_pad;
            old_pad = paddata;

            if(!collided && new_pad & PAD_CROSS)
            {
                gravity = 1;
                b->vy = -3;
            }
        }
        if(birdTouchingGround(b, top))
        {
            printf("bird hit the ground");
            while(1);
        }
        if(collision(b, pipes))
        {
            collided = 1;
        }

        // draw pipe
	drawPipes(gsGlobal, pipes, &lowerPipe);
	if(!collided)movePipes(pipes, 2, b, &score);

        //draw platform
        gsKit_prim_quad_texture(gsGlobal, &ground,
                                0.0f, 400.0f,   // x1, y1
                                0.0f, 0.0f,     // u1, v1
                                
                                0.0f, 512.0f,   // x2, y2
                                0.0f, 56.0f,    // u2, v2
                                
                                640.0f, 400.0f, // x3, y3
                                320.0f, 0.0f,   // u3, v3
                                
                                640.0f, 512.0f, // x4, y4
                                320.0f, 56.0f,  // u4, v4
                                2, TexCol);

        // draw bird
        if(gravity)
        {
            b->vy += 0.2;
            b->y += b->vy;
        }
        drawBird(gsGlobal, b, &brd);
        printScore(gsGlobal, score, &lowerPipe);
        
        gsKit_queue_exec(gsGlobal);
        gsKit_sync_flip(gsGlobal);
    }
    return 0;
}

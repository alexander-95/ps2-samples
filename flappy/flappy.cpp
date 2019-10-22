//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>
#include <time.h>
#include <stdlib.h>

#include <stdio.h>
#include <kernel.h>
#include <sifrpc.h>
#include <libpad.h>
#include <dmaKit.h>
#include <gsToolkit.h>

#include "controller.hpp"
#include "log.hpp"

#include "textures/spritesheet.h"
#include "textures/font.h"

#include "graphics.hpp"
#include "draw.hpp"
#include "thirdparty/ioman.hpp"
#include "extern_irx.h"

#include "audio/sfx_point.h"
#include "audio/sfx_wing.h"
#include "audio/sfx_hit.h"
#include "audio/sfx_die.h"
#include "audio/sfx_swooshing.h"

#include <string.h>
#include <loadfile.h>
#include <tamtypes.h>
#include <audsrv.h>

#include <iopcontrol.h>
#include <iopheap.h>
#include <sbv_patches.h>

#include <unistd.h>

#include "bird.hpp"
#include "pipeList.hpp"

static int padBuf[256] __attribute__((aligned(64)));
#define IO_CUSTOM_SIMPLEACTION 1 // handler for parameter-less actions

struct sound
{
    audsrv_adpcm_t s;
    int size;
    u8* buffer;
};

struct audioResources
{
    struct sound point, die, hit, swooshing, wing;
};

struct textureResources
{
    GSTEXTURE spriteSheet, font;
};

struct audioResources audio;


static void playPointSound(void)
{
    audsrv_ch_play_adpcm(0, &audio.point.s);
}
static void playWingSound(void)
{
    audsrv_ch_play_adpcm(1, &audio.wing.s);
}
static void playHitSound(void)
{
    audsrv_ch_play_adpcm(2, &audio.hit.s);
}
static void playDieSound(void)
{
    audsrv_ch_play_adpcm(3, &audio.die.s);
}
static void playSwooshingSound(void)
{
    audsrv_ch_play_adpcm(4, &audio.swooshing.s);
}

void loadSoundToSPU(struct sound* s)
{
    audsrv_load_adpcm(&s->s, s->buffer, s->size);
    free(s->buffer);
}

int collision(Bird* b, PipeList* pipes)
{
    Pipe* curr = pipes->head;
    for(int i=0; i<pipes->length; i++)
    {
        char xCollision = b->x+30 > curr->x && b->x < curr->x+curr->d;
        char yCollision = b->y + 24 > curr->y+50 || b->y < curr->y-50;
        if(xCollision && yCollision)return 1;
        curr = curr->next;
    }
    return 0;
}

int getHighScore()
{
    FILE* savefile = fopen("mass:flappy/savefile.txt","r");
    int score = 0;
    if(savefile)
    {
        fscanf(savefile,  "%d", &score);
        fclose(savefile);
    }
    return score;
}

void setHighScore(int score)
{
    FILE* savefile;
    savefile = fopen("mass:flappy/savefile.txt", "w");
    if(savefile)
    {
        fprintf(savefile, "%d", score);
        fclose(savefile);
    }
}

void movePipes(PipeList* ps, int val, Bird* b, int* score)
{
    Pipe* curr = ps->head;
    for(int i=0;i<ps->length;i++)
    {
        if(b->x >= curr->x && b->x < curr->x+val)(*score)++;
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
    return;
}

void pregameLoop(GSGLOBAL* gsGlobal, controller* pad, Bird* b, struct textureResources* texture, struct log* l, int nightMode, u8 fontStyle)
{
    u8 menuActive = 0;
    int cursor = 0;
    int menuItemCount = 17;
    
    while(1)
    {
        pad->read();
        if(pad->x(1)) return;

        // DEBUG MENU LOGIC
        if(pad->triangle(1))
        {
            if(!menuActive)
            {
                drawMenu(l, 10, 10, 30, 20, "DEBUG MENU", cursor);
                menuActive = 1;
            }
            else
            {
                clearMenu(l, 10, 10, 30, 20);
                menuActive = 0;
            }
        }
        if(pad->down(1))
        {
            cursor++;
            if(cursor == menuItemCount) cursor = 0;
            setCursor(l, 10, 10, 30, 20, cursor);
        }
        if(pad->up(1))
        {
            cursor--;
            if(cursor == -1) cursor = menuItemCount-1;
            setCursor(l, 10, 10, 30, 20, cursor);
        }// END DEBUG MENU LOGIC
        
        drawBackground(gsGlobal, &texture->spriteSheet, nightMode);
        b->draw();
        drawPlatform(gsGlobal, &texture->spriteSheet);
        drawGetReady(gsGlobal, &texture->spriteSheet);
        drawBuffer(gsGlobal, &texture->font, l, fontStyle);
        updateFrame(gsGlobal, &texture->font, l->buffer);
    }
}

void gameLoop(GSGLOBAL* gsGlobal, controller* pad, Bird* b, int* score, int* highScore,
              PipeList* pipes, struct audioResources* audio, struct textureResources* texture,
              struct log* l, int nightMode)
{
    int collided = 0;
    while(1)
    {
        pad->read();
        if(!collided && pad->x(1))
        {
            b->vy = -5;
            ioPutRequest(IO_CUSTOM_SIMPLEACTION, &playWingSound);
            
        }
        if(b->touchingGround())
        {
            printf("bird hit the ground\n");
            return;
        }
        if(collision(b, pipes))
        {
            // this sound file won't be loaded on a real PS2
            if(!collided)ioPutRequest(IO_CUSTOM_SIMPLEACTION, &playHitSound);
            collided = 1;
        }
        
        int oldScore = *score;
	if(!collided)movePipes(pipes, 2, b, score);
        if(*score!=oldScore)
        {
            ioPutRequest(IO_CUSTOM_SIMPLEACTION, &playPointSound);
        }

        // deal with gravity
        b->vy += 0.3;
        if(b->y + b->vy <= 380 )b->y += (int)b->vy;
        else b->y=380;
        
        drawBackground(gsGlobal, &texture->spriteSheet, nightMode);
        pipes->draw();
        b->draw();
        drawPlatform(gsGlobal, &texture->spriteSheet);
        drawScore(gsGlobal, *score, &texture->spriteSheet);
        updateFrame(gsGlobal, &texture->font, l->buffer);
    }
}

void postgameLoop(GSGLOBAL* gsGlobal, controller* pad, Bird* b, int* score, int* highScore,
                  PipeList* pipes, struct textureResources* texture, struct log* l, int nightMode)
{
    while(1)
    {
        pad->read();
        if(pad->x(1)) return;

        drawBackground(gsGlobal, &texture->spriteSheet, nightMode);
        pipes->draw();
        b->draw();
        drawPlatform(gsGlobal, &texture->spriteSheet);
        drawEnd(gsGlobal, &texture->spriteSheet, *score, *highScore);
        updateFrame(gsGlobal, &texture->font, l->buffer);
    }
}

void saveGame(GSGLOBAL* gsGlobal, Bird* b, int* score, int* highScore, PipeList* pipes,
              struct textureResources* texture, struct log* l, int nightMode)
{
    drawBackground(gsGlobal, &texture->spriteSheet, nightMode);
    pipes->draw();
    b->draw();
    drawPlatform(gsGlobal, &texture->spriteSheet);
    drawEnd(gsGlobal, &texture->spriteSheet, *score, *highScore);
    drawSaveIcon(gsGlobal, &texture->spriteSheet);
    updateFrame(gsGlobal, &texture->font, l->buffer);

    if(*score > *highScore)*highScore = *score;
    setHighScore(*highScore);
}


static void deferredAudioInit(void)
{
    audsrv_init();
    audsrv_adpcm_init();
    loadSoundToSPU(&audio.point);
    loadSoundToSPU(&audio.wing);
    loadSoundToSPU(&audio.hit);
    loadSoundToSPU(&audio.die);
    loadSoundToSPU(&audio.swooshing);
}

static void init(void)
{
    ioInit();
    ioPutRequest(IO_CUSTOM_SIMPLEACTION, &deferredAudioInit);
}


void sysReset()
{
    SifExitIopHeap();
    SifLoadFileExit();
    SifExitRpc();

    SifInitRpc(0);
    while (!SifIopReset("", 0));
    while (!SifIopSync());
    SifInitRpc(0);

    // init loadfile & iopheap services
    SifLoadFileInit();
    SifInitIopHeap();

    // apply sbv patches
    sbv_patch_enable_lmb();
    sbv_patch_disable_prefix_check();

    // load modules
    SifLoadModule("rom0:SIO2MAN", 0, NULL);
    SifLoadModule("rom0:PADMAN", 0, NULL);
    SifExecModuleBuffer(&libsd_irx, size_libsd_irx, 0, NULL, NULL);
    SifExecModuleBuffer(&audsrv_irx, size_audsrv_irx, 0, NULL, NULL);
}

void loadSound(struct sound* s, u8* data, int size)
{
    s->size = size;
    s->buffer = data;
    //s->adpcm = 1;
}

int main(int argc, char* argv[])
{
    ChangeThreadPriority(GetThreadId(), 31);

    printf("starting game\n");
    GSGLOBAL* gsGlobal = gsKit_init_global();
    configureGraphics(gsGlobal);
    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

    //Initialize the DMAC
    dmaKit_chan_init(DMA_CHANNEL_GIF);
    gsKit_init_screen(gsGlobal);
    gsKit_set_clamp(gsGlobal, GS_CMODE_CLAMP);
    struct textureResources texture;
    texture.spriteSheet = loadTexture(gsGlobal, spritesheet_array, 256, 256, GS_PSM_CT32);
    texture.font = loadTexture(gsGlobal, font_array, 128,128,GS_PSM_CT32);
    
    drawTitleScreen(gsGlobal, &texture.spriteSheet);

    struct log l;
    l.index = 0;
    l.logfile = "mass:flappy/log.txt";
    l.logToFile = 0;
    l.logToScreen = 1;
    if(l.logToFile)
    {
        FILE* f = fopen(l.logfile, "w");
        fprintf(f, "log initialised\n");
        fclose(f);
    }

    loadSound(&audio.point, sfx_point_array, 50736);
    loadSound(&audio.wing, sfx_wing_array, 8560);
    loadSound(&audio.hit, sfx_hit_array, 27632);
    loadSound(&audio.die, sfx_die_array, 38160);
    loadSound(&audio.swooshing, sfx_swooshing_array, 101360);
    
    sysReset();
    init();
    
    ioPutRequest(IO_CUSTOM_SIMPLEACTION, &playPointSound);    
    
    //controller setup
    int port=0, slot=0;
    controller pad;
    //pad.loadModules();
    padInit(0);
    pad.openPad(port,slot, padBuf);
    
    Bird bird;
    bird.gsGlobal = gsGlobal;
    bird.spritesheet = &texture.spriteSheet;
    PipeList pipes;
    pipes.gsGlobal = gsGlobal;
    pipes.spritesheet = &texture.spriteSheet;
    pipes.setup();
    int score = 0, highScore = 0, nightMode = 0;
    enum color{RED, YELLOW, BLUE};
    enum fontstyle{PLAIN, OUTLINED};
    
    highScore = getHighScore();

    while(1)
    {
        clearBuffer(&l);
        logMessage(gsGlobal, &texture.font, &l, "DEBUG: resetting score");
        score = 0;
        bird.reset(BLUE);
        pipes.reset();
        pregameLoop(gsGlobal, &pad, &bird, &texture, &l, nightMode, OUTLINED);
        bird.vy = -3;
        srand(time(0));
        gameLoop(gsGlobal, &pad, &bird, &score, &highScore, &pipes, &audio, &texture, &l, nightMode);
        postgameLoop(gsGlobal, &pad, &bird, &score, &highScore, &pipes, &texture, &l, nightMode);
        saveGame(gsGlobal, &bird, &score, &highScore, &pipes, &texture, &l, nightMode);
    }
}

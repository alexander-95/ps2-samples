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
#include <malloc.h>

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

static int padBuf[256] __attribute__((aligned(64)));
#define IO_CUSTOM_SIMPLEACTION 1 // handler for parameter-less actions

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
    int x, y, color;
    float theta;
    float vy;
    unsigned char cycle;
};

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

int collision(struct bird* b, struct pipeList* pipes)
{
    struct pipe* curr = pipes->head;
    while(curr!=NULL)
    {
        char xCollision = b->x+30 > curr->x && b->x < curr->x+curr->d;
        char yCollision = b->y + 24 > curr->y+50 || b->y < curr->y-50;
        if(xCollision &&  yCollision)return 1;
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

void drawBird(GSGLOBAL* gsGlobal, struct bird* b, GSTEXTURE* tex)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    int color = b->color;
    // offset used to cycle between bird images for wing flapping animation
    float offset = 0;
    if((b->cycle & 4) == 0)offset = 17.0f;
    else if((b->cycle & 8) == 0)offset = 0.0f;
    else offset = 34.0f;

    gsKit_prim_quad_texture(gsGlobal, tex,
                            b->x, b->y,                     // x1, y1
                            0.0f+offset, 14.0f+(12*color),  // u1, v1

                            b->x, b->y+24,                  // x2, y2
                            0.0f+offset, 26.0f+(12*color),  // u2, v2

                            b->x+34, b->y,                  // x3, y3
                            17.0f+offset, 14.0f+(12*color), // u3, v3

                            b->x+34, b->y+24,               // x4, y4
                            17.0f+offset, 26.0f+(12*color), // u4, v4
                            1, TexCol);
    b->cycle++;
    return;
}

void resetBird(struct bird* b, int color)
{
    b->x = 200;
    b->y = 200;
    b->vy = 0;
    b->cycle = 0;
    b->color = color;
}

int birdTouchingGround(struct bird* b)
{
    if(b->y >= 380)
    {
        return 1;
    }
    return 0;
}

void drawPipes(GSGLOBAL* gsGlobal, struct pipeList* ps, GSTEXTURE* spriteSheet, int nightMode)
{
    int offset = 0;
    if(nightMode)offset = 26;
    struct pipe* curr = ps->head;
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    while(curr!=NULL)
    {
        //upper pipe
        gsKit_prim_quad_texture(gsGlobal, spriteSheet,
                                curr->x, curr->y-74,         // x2, y2
                                0.0f+offset, 13.0f,          // u2, v2

                                curr->x, curr->y-50,          // x1, y1
                                0.0f+offset, 0.0f,            // u1, v1

                                curr->x+curr->d, curr->y-74, // x4, y4
                                26.0f+offset, 13.0f,         // u4, v4

                                curr->x+curr->d, curr->y-50,  // x3, y3
                                26.0f+offset, 0.0f,           // u3, v3

                                1, TexCol);
        
        gsKit_prim_quad_texture(gsGlobal, spriteSheet,
                                curr->x, curr->y-450,         // x2, y2
                                0.0f+offset, 14.0f,          // u2, v2

                                curr->x, curr->y-74,          // x1, y1
                                0.0f+offset, 13.0f,            // u1, v1

                                curr->x+curr->d, curr->y-450, // x4, y4
                                26.0f+offset, 14.0f,         // u4, v4

                                curr->x+curr->d, curr->y-74,  // x3, y3
                                26.0f+offset, 13.0f,           // u3, v3
                                1, TexCol);
        
        // lower pipe
        gsKit_prim_quad_texture(gsGlobal, spriteSheet,
                                curr->x, curr->y+50,          // x1, y1
                                0.0f+offset, 0.0f,            // u1, v1

                                curr->x, curr->y+74,         // x2, y2
                                0.0f+offset, 13.0f,          // u2, v2

                                curr->x+curr->d, curr->y+50,  // x3, y3
                                26.0f+offset, 0.0f,           // u3, v3

                                curr->x+curr->d, curr->y+74, // x4, y4
                                26.0f+offset, 13.0f,         // u4, v4
                                1, TexCol);
        
        gsKit_prim_quad_texture(gsGlobal, spriteSheet,
                                curr->x, curr->y+74,          // x1, y1
                                0.0f+offset, 13.0f,            // u1, v1

                                curr->x, curr->y+450,         // x2, y2
                                0.0f+offset, 14.0f,          // u2, v2

                                curr->x+curr->d, curr->y+74,  // x3, y3
                                26.0f+offset, 13.0f,           // u3, v3

                                curr->x+curr->d, curr->y+450, // x4, y4
                                26.0f+offset, 14.0f,         // u4, v4
                                1, TexCol);
        
        curr = curr->next;
    }
    return;
}

void movePipes(struct pipeList* ps, int val, struct bird* b, int* score)
{
    struct pipe* curr = ps->head;
    int i;
    for(i=0;i<ps->length;i++)
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

void resetPipes(struct pipeList* pipes)
{
    struct pipe* curr = pipes->head;
    int i;
    for(i=0;i<pipes->length;i++)
    {
        curr->x = 640+pipes->gap*(i), curr->y = rand() % 300 + 50, curr->d = 52;
        curr = curr->next;
    }
}

struct pipeList* setupPipes()
{
    int i;
    struct pipeList* pipes = (struct pipeList*) malloc(sizeof(struct pipeList));
    pipes->gap = 200;
    struct pipe* curr = (struct pipe*) malloc(sizeof(struct pipe));
    pipes->head = curr;
    pipes->length = 4;
    for(i=0;i<pipes->length-1;i++)
    {
        struct pipe* p = (struct pipe*) malloc(sizeof(struct pipe));
        curr->next = p;
        p->prev = curr;
        curr->x = 640+pipes->gap*(i), curr->y = rand() % 300 + 50, curr->d = 52;
        curr = curr->next;
    }
    return pipes;
}

void pregameLoop(GSGLOBAL* gsGlobal, struct controller* pad1, struct bird* b, struct textureResources* texture, char* buffer, int nightMode, u8 fontStyle)
{
    while(1)
    {
        padUpdate(pad1);
        if(pad1->new_pad & PAD_CROSS) return;

        drawBackground(gsGlobal, &texture->spriteSheet, nightMode);
        drawBird(gsGlobal, b, &texture->spriteSheet);
        drawPlatform(gsGlobal, &texture->spriteSheet);
        drawGetReady(gsGlobal, &texture->spriteSheet);
        drawBuffer(gsGlobal, &texture->font, buffer, fontStyle);
        updateFrame(gsGlobal, &texture->font, buffer);
    }
}

void gameLoop(GSGLOBAL* gsGlobal, struct controller* pad1, struct bird* b, int* score, int* highScore, struct pipeList* pipes,
              struct audioResources* audio, struct textureResources* texture, char* buffer, int nightMode)
{
    int collided = 0;
    while(1)
    {
        padUpdate(pad1);
        if(!collided && pad1->new_pad & PAD_CROSS)
        {
            b->vy = -5;
            ioPutRequest(IO_CUSTOM_SIMPLEACTION, &playWingSound);
            
        }
        if(birdTouchingGround(b))
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
        drawPipes(gsGlobal, pipes, &texture->spriteSheet, nightMode);
        drawBird(gsGlobal, b, &texture->spriteSheet);
        drawPlatform(gsGlobal, &texture->spriteSheet);
        drawScore(gsGlobal, *score, &texture->spriteSheet);
        updateFrame(gsGlobal, &texture->font, buffer);
    }
}

void postgameLoop(GSGLOBAL* gsGlobal, struct controller* pad1, struct bird* b, int* score, int* highScore, struct pipeList* pipes,
                  struct textureResources* texture, char* buffer, int nightMode)
{
    while(1)
    {
        padUpdate(pad1);
        if(pad1->new_pad & PAD_CROSS) return;

        drawBackground(gsGlobal, &texture->spriteSheet, nightMode);
        drawPipes(gsGlobal, pipes, &texture->spriteSheet, nightMode);
        drawBird(gsGlobal, b, &texture->spriteSheet);
        drawPlatform(gsGlobal, &texture->spriteSheet);
        drawEnd(gsGlobal, &texture->spriteSheet, *score, *highScore);
        updateFrame(gsGlobal, &texture->font, buffer);
    }
}

void saveGame(GSGLOBAL* gsGlobal, struct bird* b, int* score, int* highScore, struct pipeList* pipes,
              struct textureResources* texture, char* buffer, int nightMode)
{
    drawBackground(gsGlobal, &texture->spriteSheet, nightMode);

    drawPipes(gsGlobal, pipes, &texture->spriteSheet, nightMode);
    drawBird(gsGlobal, b, &texture->spriteSheet);
    drawPlatform(gsGlobal, &texture->spriteSheet);
    drawEnd(gsGlobal, &texture->spriteSheet, *score, *highScore);
    drawSaveIcon(gsGlobal, &texture->spriteSheet);
    updateFrame(gsGlobal, &texture->font, buffer);

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
    
    struct controller pad1 = setupController(padBuf);
    struct bird* b = (struct bird*) malloc(sizeof(struct bird));
    struct pipeList* pipes = setupPipes();
    int score = 0, highScore = 0, nightMode = 0;
    enum color{RED, YELLOW, BLUE};
    enum fontstyle{PLAIN, OUTLINED};
    
    highScore = getHighScore();

    while(1)
    {
        logMessage(gsGlobal, &texture.font, &l, "DEBUG: resetting score");
        score = 0;
        resetBird(b, BLUE);
        resetPipes(pipes);
        pregameLoop(gsGlobal, &pad1, b, &texture, l.buffer, nightMode, OUTLINED);
        b->vy = -3;
        srand(time(0));
        gameLoop(gsGlobal, &pad1, b, &score, &highScore, pipes, &audio, &texture, l.buffer, nightMode);
        postgameLoop(gsGlobal, &pad1, b, &score, &highScore, pipes, &texture, l.buffer, nightMode);
        saveGame(gsGlobal, b, &score, &highScore, pipes, &texture, l.buffer, nightMode);
    }
}

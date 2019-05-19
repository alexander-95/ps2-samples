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
#include "log.h"

#include "bg.h"
#include "spritesheet.h"
#include "font.h"
#include "graphics.h"
#include "draw.h"

#include <string.h>
#include <loadfile.h>
#include <tamtypes.h>
#include <audsrv.h>

u8 PCSX2 = 1;
static int padBuf[256] __attribute__((aligned(64)));

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
    unsigned char cycle;
};

struct sound
{
    FILE* adpcm;
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
    GSTEXTURE bg, spriteSheet, font;
};

void loadAudioModules()
{
    int ret;
    printf("sample: kicking IRXs\n");
    ret = SifLoadModule("rom:LIBSD", 0, NULL);
    if (ret < 0)ret = SifLoadModule("host:LIBSD", 0, NULL);
    printf("libsd loadmodule %d\n", ret);

    printf("sample: loading audsrv\n");
    if(PCSX2) ret = SifLoadModule("host:audsrv.irx", 0, NULL);
    else ret = SifLoadModule("mass:flappy/audsrv.irx", 0, NULL);
    printf("audsrv loadmodule %d\n", ret);
}

void loadSound(struct sound* s, char* c, GSGLOBAL* gsGlobal, GSTEXTURE* font, struct log* l)
{
    char* filename = (char*)malloc(50 * sizeof(char));
    if(PCSX2) sprintf(filename, "host:%s", c);
    else sprintf(filename, "mass:flappy/%s", c);
    s->adpcm = NULL;
    while(s->adpcm == NULL)s->adpcm = fopen(filename, "rb");

    logMessage(gsGlobal, font, l, filename);

    if(0 && s->adpcm == NULL)
    {
        printf("failed to open adpcm file\n");
        audsrv_quit();
    }

    fseek(s->adpcm, 0, SEEK_END);
    s->size = ftell(s->adpcm);
    fseek(s->adpcm, 0, SEEK_SET);
    char str[45];
    sprintf(str, "attempting malloc %d", s->size);
    logMessage(gsGlobal, font, l, str);
    s->buffer = malloc(s->size);
    logMessage(gsGlobal, font, l, "success. reading adpcm");
    fread(s->buffer, 1, s->size, s->adpcm);
    logMessage(gsGlobal, font, l, "adpcm loaded into buffer");
    fclose(s->adpcm);

    logMessage(gsGlobal, font, l, "file loaded from buffer");
    
    audsrv_load_adpcm(&s->s, s->buffer, s->size);
    logMessage(gsGlobal, font, l, "audio loaded to spu");
    free(s->buffer);
}

int initialiseAudio()
{
    if(audsrv_init() != 0)
    {
        printf("sample: failed to initialize audsrv\n");
        printf("audsrv returned error string: %s\n", audsrv_get_error_string());
        return 1;
    }
    audsrv_adpcm_init();
    audsrv_set_volume(MAX_VOLUME);
    return 0;
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
    int color = 0; // 0 = red, 1 = yellow, 2 = blue
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    float offset = 0;
    if((b->cycle & 4) == 0)offset = 17.0f;
    else if((b->cycle & 8) == 0)offset = 0.0f;
    else offset = 34.0f;
    offset += (color*51);

    gsKit_prim_quad_texture(gsGlobal, tex,
                            b->x, b->y,          // x1, y1
                            52.0f+offset, 18.0f, // u1, v1

                            b->x, b->y+24,       // x2, y2
                            52.0f+offset, 30.0f, // u2, v2

                            b->x+34, b->y,       // x3, y3
                            69.0f+offset, 18.0f, // u3, v3

                            b->x+34, b->y+24,    // x4, y4
                            69.0f+offset, 30.0f, // u4, v4
                            1, TexCol);
    b->cycle++;
    return;
}

void resetBird(struct bird* b)
{
    b->x = 200;
    b->y = 200;
    b->vy = 0;
    b->cycle = 0;
}

int birdTouchingGround(struct bird* b)
{
    if(b->y >= 380)
    {
        return 1;
    }
    return 0;
}

void drawPipes(GSGLOBAL* gsGlobal, struct pipeList* ps, GSTEXTURE* spriteSheet)
{
    int daytime = 0, offset = 0;
    if(daytime)offset = 26;
    struct pipe* curr = ps->head;
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    while(curr!=NULL)
    {
        //upper pipe
        gsKit_prim_quad_texture(gsGlobal, spriteSheet,
                                curr->x, curr->y-450,         // x1, y1
                                0.0f+offset, 200.0f,          // u1, v1

                                curr->x, curr->y-50,          // x2, y2
                                0.0f+offset, 0.0f,            // u2, v2

                                curr->x+curr->d, curr->y-450, // x3, y3
                                26.0f+offset, 200.0f,         // u3, v3

                                curr->x+curr->d, curr->y-50,  // x4, y4
                                26.0f+offset, 0.0f,           // u4, v4
                                1, TexCol);
        
        // lower pipe
        gsKit_prim_quad_texture(gsGlobal, spriteSheet,
                                curr->x, curr->y+50,          // x1, y1
                                0.0f+offset, 0.0f,            // u1, v1

                                curr->x, curr->y+450,         // x2, y2
                                0.0f+offset, 200.0f,          // u2, v2

                                curr->x+curr->d, curr->y+50,  // x3, y3
                                26.0f+offset, 0.0f,           // u3, v3

                                curr->x+curr->d, curr->y+450, // x4, y4
                                26.0f+offset, 200.0f,         // u4, v4
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
    return pipes;
}

void pregameLoop(GSGLOBAL* gsGlobal, struct controller* pad1, struct bird* b, struct textureResources* texture, char* buffer)
{
    while(1)
    {
        padUpdate(pad1);
        if(pad1->new_pad & PAD_CROSS) return;

        drawBackground(gsGlobal, &texture->bg);
        drawBird(gsGlobal, b, &texture->spriteSheet);
        drawPlatform(gsGlobal, &texture->spriteSheet);
        drawGetReady(gsGlobal, &texture->spriteSheet);
        updateFrame(gsGlobal, &texture->font, buffer);
    }
}

void gameLoop(GSGLOBAL* gsGlobal, struct controller* pad1, struct bird* b, int* score, int* highScore, struct pipeList* pipes,
              struct audioResources* audio, struct textureResources* texture, char* buffer)
{
    int collided = 0;
    while(1)
    {
        padUpdate(pad1);
        if(!collided && pad1->new_pad & PAD_CROSS)
        {
            b->vy = -5;
            audsrv_play_adpcm(&audio->wing.s);
        }
        if(birdTouchingGround(b))
        {
            printf("bird hit the ground\n");
            return;
        }
        if(collision(b, pipes))
        {
            // this sound file won't be loaded on a real PS2
            if(!collided && PCSX2)audsrv_play_adpcm(&audio->hit.s);
            collided = 1;
        }
        
        int oldScore = *score;
	if(!collided)movePipes(pipes, 2, b, score);

        if(*score!=oldScore)
        {
            audsrv_play_adpcm(&audio->point.s);
        }

        // deal with gravity
        b->vy += 0.3;
        if(b->y + b->vy <= 380 )b->y += b->vy;
        else b->y=380;

        drawBackground(gsGlobal, &texture->bg);
        drawPipes(gsGlobal, pipes, &texture->spriteSheet);
        drawBird(gsGlobal, b, &texture->spriteSheet);
        drawPlatform(gsGlobal, &texture->spriteSheet);
        drawScore(gsGlobal, *score, &texture->spriteSheet);
        updateFrame(gsGlobal, &texture->font, buffer);
    }
}

void postgameLoop(GSGLOBAL* gsGlobal, struct controller* pad1, struct bird* b, int* score, int* highScore, struct pipeList* pipes,
                  struct textureResources* texture, char* buffer)
{
    while(1)
    {
        padUpdate(pad1);
        if(pad1->new_pad & PAD_CROSS) return;

        drawBackground(gsGlobal, &texture->bg);
        drawPipes(gsGlobal, pipes, &texture->spriteSheet);
        drawBird(gsGlobal, b, &texture->spriteSheet);
        drawPlatform(gsGlobal, &texture->spriteSheet);
        drawEnd(gsGlobal, &texture->spriteSheet, *score, *highScore);
        updateFrame(gsGlobal, &texture->font, buffer);
    }
}

void saveGame(GSGLOBAL* gsGlobal, struct bird* b, int* score, int* highScore, struct pipeList* pipes,
              struct textureResources* texture, char* buffer)
{
    drawBackground(gsGlobal, &texture->bg);
    drawPipes(gsGlobal, pipes, &texture->spriteSheet);
    drawBird(gsGlobal, b, &texture->spriteSheet);
    drawPlatform(gsGlobal, &texture->spriteSheet);
    drawEnd(gsGlobal, &texture->spriteSheet, *score, *highScore);
    drawSaveIcon(gsGlobal, &texture->spriteSheet);
    updateFrame(gsGlobal, &texture->font, buffer);

    if(*score > *highScore)*highScore = *score;
    setHighScore(*highScore);
}

int main(int argc, char* argv[])
{
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
    texture.bg = loadTexture(gsGlobal, bg_array, 320, 256, GS_PSM_CT24);    
    texture.spriteSheet = loadTexture(gsGlobal, spritesheet_array, 320, 256, GS_PSM_CT32);
    texture.font = loadTexture(gsGlobal, font_array, 256,128,GS_PSM_CT32);
    
    drawTitleScreen(gsGlobal, &texture.spriteSheet);
    
    struct controller pad1 = setupController(padBuf);
    struct bird* b = malloc(sizeof(struct bird));
    struct pipeList* pipes = setupPipes();
    int score = 0, highScore = 0;

    struct log l;
    l.index = 0;

    struct audioResources audio;
    loadAudioModules();
    if(initialiseAudio() != 0)return 1;
    loadSound(&audio.point, "sfx_point.adp", gsGlobal, &texture.font, &l);
    
    loadSound(&audio.wing, "sfx_wing.adp", gsGlobal, &texture.font, &l);
    // loading more files not working on real PS2 for some reason
    if(PCSX2)
    {
        loadSound(&audio.hit, "sfx_hit.adp", gsGlobal, &texture.font, &l);
        loadSound(&audio.die, "sfx_die.adp", gsGlobal, &texture.font, &l);
        loadSound(&audio.swooshing, "sfx_swooshing.adp", gsGlobal, &texture.font, &l);
    }
    
    highScore = getHighScore();

    while(1)
    {
        score = 0;
        resetBird(b);
        resetPipes(pipes);
        pregameLoop(gsGlobal, &pad1, b, &texture, l.buffer);
        b->vy = -3;
        srand(time(0));
        gameLoop(gsGlobal, &pad1, b, &score, &highScore, pipes, &audio, &texture, l.buffer);
        postgameLoop(gsGlobal, &pad1, b, &score, &highScore, pipes, &texture, l.buffer);
        saveGame(gsGlobal, b, &score, &highScore, pipes, &texture, l.buffer);
    }
}

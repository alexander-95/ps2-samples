//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>

#include "textures/spritesheet.h"
#include "textures/mario.h"
#include "textures/goomba.h"
#include "textures/koopa.h"
#include "textures/pickups.h"
#include "textures/hud.h"
#include "textures/title.h"
#include "textures/cursor.h"

#include "controller.hpp"
#include "map_data.h"
#include "character.hpp"
#include "block.hpp"
#include "pickup.hpp"
#include "HUD.hpp"

#include "levelBuilder.hpp"
#include "utils.h"

void gsKit_texture_abgr(GSGLOBAL* gsGlobal, GSTEXTURE* tex, u32* arr, u32 width, u32 height)
{
    u32 VramTextureSize = gsKit_texture_size(width, height, GS_PSM_CT32);
    u32 size = VramTextureSize;
    char* units[3] = {"b", "kb", "mb"};
    u8 unit = 0;
    while(size > 1024)
    {
        size /= 1024;
        unit++;
    }
    printf("allocating %d%s\n", size,units[unit]);

    tex->Width = width;
    tex->Height = height;
    tex->PSM = GS_PSM_CT32;
    tex->ClutPSM = 0;
    tex->TBW = 1;
    tex->Mem = arr;
    tex->Clut = NULL;
    tex->Vram = gsKit_vram_alloc(gsGlobal, VramTextureSize, GSKIT_ALLOC_USERBUFFER);
    tex->VramClut = 0;
    tex->Filter = GS_FILTER_NEAREST;
    tex->Delayed = 0;

    gsKit_texture_upload(gsGlobal, tex);
}

GSTEXTURE loadTexture(GSGLOBAL* gsGlobal, u32* tex_array, int width, int height)
{
    GSTEXTURE tex;
    tex.Width=width;
    tex.Height=height;
    tex.PSM = GS_PSM_CT32;
    gsKit_texture_abgr(gsGlobal, &tex, tex_array, width, height );
    return tex;
}

void drawTile(GSGLOBAL* gsGlobal, GSTEXTURE* spritesheet, int scale_factor, LevelBuilderBase* level, point start, int value, int z)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);// set color
    int index_x = value % 8; // 8 is the width of the tilesheet, in tiles
    int index_y = value / 8; // 8 is the width of the tilesheet, in tiles

    int x1 = start.x*scale_factor;
    int y1 = start.y*scale_factor;

    int x2 = (start.x + level->tile_width)*scale_factor;
    int y2 = (start.y + level->tile_height)*scale_factor;

    int u1 = level->tile_width*index_x;
    int v1 = level->tile_height*index_y;

    int u2 = level->tile_width*(index_x+1);
    int v2 = level->tile_height*(index_y+1);
    
    gsKit_prim_quad_texture(gsGlobal, spritesheet,
                            x1, y1, u1, v1, 
                            x1, y2, u1, v2,
                            x2, y1, u2, v1,
                            x2, y2, u2, v2,
                            z,TexCol);
}

void drawScreen(GSGLOBAL* gsGlobal, GSTEXTURE* spritesheet, int scale_factor, LevelBuilderBase* level, point viewport)
{
    // figure out the tile to draw
    point tile;
    tile.x = viewport.x / level->tile_width;
    tile.y = viewport.y / level->tile_height;

    // where do we start in the tile
    point point1;
    point1.x = viewport.x % level->tile_width;
    point1.y = viewport.y % level->tile_height;

    // position of the top left corner of the current tile
    point start;
    start.x = 0 - point1.x;
    start.y = 0 - point1.y;

    u8 rowsDrawn = 0;

    while(start.y < gsGlobal->Height && tile.y < level->absoluteHeight && rowsDrawn < level->height)
    {
        while(start.x < gsGlobal->Width && tile.x < level->width)
        {
            int index = tile.y * level->width + tile.x;
            int value = level->data[index];
            int z = level->solid[value]*2;
            drawTile(gsGlobal, spritesheet, scale_factor, level, start, value, z);
            start.x += level->tile_width;
            tile.x++;
        }
        start.x = 0 - point1.x;
        tile.x = viewport.x / level->tile_width;
        start.y += level->tile_height;
        rowsDrawn++;
        tile.y++;
    }
}

void drawLevelStart(GSGLOBAL* gsGlobal, HUD* hud, PlayableCharacter* mario, int score, int lives)
{
    mario->worldCoordinates.x = 128;
    mario->worldCoordinates.y = 128;
    for(u16 tick = 0; tick < 128; tick++)
    {
        hud->draw(0, 0);
        hud->drawScore(score);
        hud->drawWorld(1, 1);
        mario->draw();
        hud->drawLives(lives);
        gsKit_sync_flip(gsGlobal);
        gsKit_queue_exec(gsGlobal);
        gsKit_queue_reset(gsGlobal->Per_Queue);
        gsKit_clear(gsGlobal, 0);
    }
    printf("game started\n");
}

void drawStartScreen(GSGLOBAL* gsGlobal, controller* pad, HUD* hud, LevelBuilder_1_1* level1)
{
    u64 bg_color = GS_SETREG_RGBAQ(0x5C,0x94,0xFC,0x00,0x00);
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    GSTEXTURE title;
    GSTEXTURE cursor;
    gsKit_texture_abgr(gsGlobal, &title, title_array, 176, 88 );
    gsKit_texture_abgr(gsGlobal, &cursor, cursor_array, 8, 8 );
    point p1; p1.x = 160, p1.y = 60;
    point p2; p2.x = 210, p2.y = 250;

    point start; start.x = 0, start.y = 0;

    int x1 = 160;
    int y1 = 60;
    int u1 = 0;
    int v1 = 0;

    int x2 = x1+(title.Width * 2);
    int y2 = y1+(title.Height * 2);
    int u2 = title.Width;
    int v2 = title.Height;

    int x3 = 210;
    int y3 = 250;
    int u3 = 0;
    int v3 = 0;

    int x4 = x3+(cursor.Width * 2);
    int y4 = y3+(cursor.Height * 2);
    int u4 = cursor.Width;
    int v4 = cursor.Height;

    u8 menu_option = 0;
    while(1)
    {
        pad->read();
        if(pad->down())
        {
            menu_option = 1;
            y3 = 282;
            y4 = y3 + (cursor.Height * 2);
        }
        else if(pad->up())
        {
            menu_option = 0;
            y3 = 250;
            y4 = y3 + (cursor.Height * 2);
        }
        if(pad->x(1) && menu_option == 0)break;

        // draw "SUPER MARIO BROS." logo
        gsKit_prim_quad_texture(gsGlobal, &title,
                                x1, y1, u1, v1,
                                x1, y2, u1, v2,
                                x2, y1, u2, v1,
                                x2, y2, u2, v2,
                                5, TexCol);

        // draw goomba cursor
        gsKit_prim_quad_texture(gsGlobal, &cursor,
                                x3, y3, u3, v3,
                                x3, y4, u3, v4,
                                x4, y3, u4, v3,
                                x4, y4, u4, v4,
                                5, TexCol);
        
        drawScreen(gsGlobal, level1->spritesheet, 2, level1, start);
        hud->drawDigit(250,250, 1);
        hud->drawString(282,250, "PLAYER GAME");
        hud->drawDigit(250,280, 2);
        hud->drawString(282,280, "PLAYER GAME");
        
        gsKit_sync_flip(gsGlobal);
        gsKit_queue_exec(gsGlobal);
        gsKit_queue_reset(gsGlobal->Per_Queue);
        gsKit_clear(gsGlobal, bg_color);
    }
}

GSGLOBAL* character::gsGlobal;
GSGLOBAL* pickup::gsGlobal;
GSGLOBAL* block::gsGlobal;
GSGLOBAL* HUD::gsGlobal;

point* character::viewport;
point* pickup::viewport;
point* block::viewport;

int main()
{   
    GSGLOBAL* gsGlobal = gsKit_init_global();
    gsGlobal->Mode = GS_MODE_PAL;
    gsGlobal->Width=640;
    gsGlobal->Height=512;
    //gsGlobal->PSMZ = GS_PSMZ_32;
    gsGlobal->PSM = GS_PSM_CT16;
    gsGlobal->ZBuffering = GS_SETTING_ON;
    gsGlobal->PrimAlphaEnable = GS_SETTING_ON;
    gsGlobal->DoubleBuffering = GS_SETTING_ON;

    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

    //Initialize the DMAC
    dmaKit_chan_init(DMA_CHANNEL_GIF);
    gsKit_init_screen(gsGlobal);
    gsKit_set_clamp(gsGlobal, GS_CMODE_CLAMP);
    gsKit_mode_switch(gsGlobal, GS_ONESHOT);
    
    //controller setup
    static int padBuf[256] __attribute__((aligned(64)));
    int port=0, slot=0;
    controller pad;
    pad.loadModules();
    padInit(0);
    pad.openPad(port,slot, padBuf);

    // place the top corner of the screen at this location of the map and render the first tile.
    // int x = 0, y=0; // top left corner of the screen
    point viewport;
    viewport.x = 0;
    viewport.y = 0;
    
    int scale_factor = 2;
    u64 bg_color = GS_SETREG_RGBAQ(0x5C,0x94,0xFC,0x00,0x00);

    GSTEXTURE hudTexture = loadTexture(gsGlobal, hud_array, 128, 128);
    GSTEXTURE tilesheet = loadTexture(gsGlobal, spritesheet_array, 128, 128 );
    GSTEXTURE marioSprites = loadTexture(gsGlobal, mario_array, 512, 32 );
    GSTEXTURE pickupTexture = loadTexture(gsGlobal, pickups_array, 64, 64);
    GSTEXTURE koopaSprites = loadTexture(gsGlobal, koopa_array, 96, 24 );
    GSTEXTURE goombaSprites = loadTexture(gsGlobal, goomba_array, 32, 16);
    u8 tilesheet_solid[64] = {0,1,0,0,0,0,0,0,
                              0,0,0,1,1,1,0,0,
                              1,1,0,0,0,0,0,0,
                              1,1,0,0,0,0,0,0,
                              1,1,1,1,0,0,0,0,
                              1,1,1,0,0,0,0,0,
                              1,1,1,0,0,0,0,0,
                              0,0,0,0,0,0,0,0};

    
    HUD hud;
    HUD::gsGlobal = gsGlobal;
    hud.spritesheet = &hudTexture;
    
    character::gsGlobal = gsGlobal;
    character::viewport = &viewport;
    
    //character mario;
    PlayableCharacter mario;
    mario.spritesheet = &marioSprites;

    pickup::gsGlobal = gsGlobal;
    pickup::viewport = &viewport;

    LevelBuilder_1_1 level;
    Coin* coin = level.loadCoins(&pickupTexture);    
    Mushroom* mushroom = level.loadMushrooms(&pickupTexture);
    Flower* flower = level.loadFlowers(&pickupTexture);
    character* goomba = level.loadGoombas(&goombaSprites);
    character* koopa = level.loadKoopas(&koopaSprites);
    level.spritesheet = &tilesheet;
    level.data = map_data;
    level.solid = tilesheet_solid;
    
    // this will represent the box that was hit
    block block1;
    block::gsGlobal = gsGlobal;
    block::viewport = &viewport;
    
    mario.vy = 0;
    int gravity = 1;
    u8 tick = 0;
    u8 superMario = 0;
    int time = 400;
    int score = 0;
    int lives = 3;
    u8 frameByFrame = 0;
    u8 restart = 0;
    
    drawStartScreen(gsGlobal, &pad, &hud, &level);
    
    drawLevelStart(gsGlobal, &hud, &mario, score, lives);
    mario.worldCoordinates.x = 0;
    mario.worldCoordinates.y = 192;
    
    while(1)
    {
        // set the background color
        if(viewport.y == 0)bg_color = GS_SETREG_RGBAQ(0x5C,0x94,0xFC,0x00,0x00); // above ground
        else bg_color = GS_SETREG_RGBAQ(0x00,0x00,0x00,0x00,0x00); // underground (in cave)
        gsKit_clear(gsGlobal, bg_color);

        pad.read();
        if(!mario.canMoveDown(&level, 1) && !mario.animationMode)mario.sprite = 0;
        else if(!mario.animationMode) mario.sprite = 5;
        // mario fell into a pit
        if(!mario.animationMode && mario.worldCoordinates.y > viewport.y + 208)
        {
            printf("fell\n");
            mario.animationMode = 4;
        }
        if(mario.pipeOnRight(&level))mario.animationMode = 5;

        if(!mario.animationMode)
        {
            mario.reactToControllerInput(&pad, tick, &level, scale_factor, &superMario, &frameByFrame);
        }
        else
        {
            mario.doAnimation(tick, &superMario, &restart);
        }
        
        if(restart)
        {
            mario.animationMode = 0;
            mario.animationFrame = 0;
            mario.sprite = 0;
            drawLevelStart(gsGlobal, &hud, &mario, score, lives);
            mario.worldCoordinates.x = 0;
            viewport.x = 0;
            mario.worldCoordinates.y = 192;
            mario.collisionDetection = 1;
            restart = 0;
        }
                
        // dealing with gravity
        if(mario.vy > 0)
        {
            // did mario stomp any goombas?
            for(int i = 0; i < 16; i++)
            {
                if(goomba[i].isOnScreen() && mario.isTouching(&goomba[i]))
                {
                    goomba[i].sprite = 1;
                    mario.vy = -6;
                    goomba[i].activated = 0;
                    goomba[i].collisionDetection = 0;
                }
            }
            
            if(!mario.animationMode && mario.canMoveDown(&level, mario.vy))
            {
                mario.worldCoordinates.y += mario.vy;
                if((tick & 3) == 0)mario.vy += gravity;
            }
            else if(!mario.animationMode)
            {
                while(mario.vy > 0 && !mario.canMoveDown(&level, mario.vy))mario.vy--;
                mario.worldCoordinates.y += mario.vy;
                mario.vy = 0;
                
                mario.sprite = 0;
            }
        }
        else if(mario.vy < 0 || mario.canMoveDown(&level, 1)) // jumping
        {
            if(mario.canMoveUp(&level, mario.vy*(-1)))
            {
                mario.worldCoordinates.y += mario.vy;
                if((tick & 3) == 0)mario.vy += gravity;
            }
            else
            {
                while(mario.vy < 0 && !mario.canMoveUp(&level, mario.vy*(-1)))mario.vy++;
                if(mario.vy == 0)
                {
                    int index = level.get_index(mario.worldCoordinates.x + 8, mario.worldCoordinates.y - 1);
                    int coinx = ((index % level.width) * 16)+4;
                    int coiny = (index / level.width) * 16;
                    printf("hit box %d (%d) coin location: <%d, %d>\n", index,level.data[index], coinx, coiny);
                    if(level.data[index] == 11 || level.data[index] == 33)
                    {
                        block1.sprite = 32;
                        block1.x = ((mario.worldCoordinates.x + 8) >> 4)<<4;
                        block1.y = ((mario.worldCoordinates.y - 1) >> 4)<<4;
                        block1.spritesheet = level.spritesheet;
                        level.data[index] = 0;
                        block1.active = 1;
                        printf("viewport: <%d, %d>", viewport.x, viewport.y);
                        printf("<%d, %d>\n", block1.x, block1.y);
                        if(index == 2032){coin[0].activated = 1;score+=200;}
                        else if(index == 2037)mushroom[0].activated = 1;
                        else if(index == 2039){coin[1].activated = 1;score+=200;}
                        else if(index == 1142){coin[2].activated = 1;score+=200;}
                        else if(index == 1214){coin[3].activated = 1;score+=200;}
                        else if(index == 2122){coin[4].activated = 1;score+=200;}
                        else if(index == 2125){coin[5].activated = 1;score+=200;}
                        else if(index == 2128){coin[6].activated = 1;score+=200;}
                        else if(index == 1249){coin[7].activated = 1;score+=200;}
                        else if(index == 1250){coin[8].activated = 1;score+=200;}
                        else if(index == 2186){coin[9].activated = 1;score+=200;}
                        else if(index == 2094)flower[0].activated = 1;
                        else if(index == 1229)flower[1].activated = 1;
                        else if(index == 1856)mushroom[1].activated = 1;
                    }
                }
                mario.worldCoordinates.y += mario.vy;
            }
        }
        else if(mario.vy == 0) // mario is only able to get killed by enemies when not jumping or falling
        {
            for(int i = 0; i < 16; i++)
            {
                if(mario.animationMode == 0 && goomba[i].isOnScreen() && mario.isTouching(&goomba[i]))
                {
                    printf("touching a goomba\n");
                    mario.animationMode = 4;
                }
            }
        }
        if(superMario)mario.sprite+=15;
        drawScreen(gsGlobal, level.spritesheet, scale_factor, &level, viewport);
        mario.draw();
        for(int i = 0; i < 32; i++)
        {
            coin[i].draw();
            if(i < 10)coin[i].update();
            if(mario.pickedup(&coin[i]))
            {
                printf("picked up a coin\n");
                coin[i].activated = 0;
                score+=200;
            }
        }
        for(int i = 0; i < 4; i++)
        {
            if(mario.pickedup(&mushroom[0]))
            {
                printf("picked up mushroom\n");
                mushroom[i].activated = 0;
                mario.animationMode = 2;
            }
            mushroom[i].draw();
            mushroom[i].update();
        }
        for(int i = 0; i < 2; i++)
        {
            flower[i].draw();
            if((tick & 7)==0)
            {
                flower[i].update();
            }
            if(tick&1)
            {
                flower[i].sprite++;
                flower[i].sprite&=3;
            }
        }
        for(int i = 0;i < 16; i++)
        {
            if(goomba[i].isOnScreen())goomba[i].draw();
        }
        if(koopa[0].isOnScreen())koopa[0].draw();

        if(block1.active)
        {
            block1.draw();
            if(tick&1)
            {
                if(block1.update())
                {
                    int index_x = block1.x / level.tile_width;
                    int index_y = block1.y / level.tile_height;
                    printf("box location: <%d, %d>\n", block1.x, block1.y);
                    block1.active = 0;
                    block1.phase = 0;
                    level.data[(index_y*level.width)+index_x] = 32;
                }
            }
        }
        hud.draw(0, 0);
        hud.drawTime(time);
        hud.drawScore(score);
        hud.drawWorld(1, 1);
        
        gsKit_sync_flip(gsGlobal);
        gsKit_queue_exec(gsGlobal);
        gsKit_queue_reset(gsGlobal->Per_Queue);
        gsKit_clear(gsGlobal, 0);
        if((tick&3) == 0) // every 4 frames
        {
            if((tick&7) == 0) // every 8 frames
            {
                for(int i = 0; i < 16; i++)
                {
                    if(goomba[i].isOnScreen())goomba[i].hflip^=1;
                }
                for(int i = 0; i < 32; i++)
                {
                    coin[i].sprite++;
                    coin[i].sprite&=3;
                }
            }
            for(int i = 0; i < 16; i++)
            {
                if(goomba[i].isOnScreen())
                {
                    goomba[i].traverse(&level);
                    goomba[i].gravity(&level,tick, gravity);
                }
            }
            koopa[0].traverse(&level);
            koopa[0].gravity(&level,tick, gravity);
            koopa[0].hflip = koopa[0].direction ^ 1;
            if((tick&7) == 0)koopa[0].sprite = 2;
            else koopa[0].sprite = 3;            
        }
        if((tick&31) == 0)time--; // every 32 frames
        tick++;
    }
    return 0;
}

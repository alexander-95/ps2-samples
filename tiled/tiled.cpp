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
#include "map.hpp"
#include "map_data.h"
#include "character.hpp"
#include "block.hpp"
#include "pickup.hpp"
#include "HUD.hpp"

typedef struct point
{
    int x;
    int y;
}point;

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

void drawTile(GSGLOBAL* gsGlobal, GSTEXTURE* spritesheet, int scale_factor, map* level, point start, int value, int z)
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

void drawScreen(GSGLOBAL* gsGlobal, GSTEXTURE* spritesheet, int scale_factor, map* level, point viewport, u8* map_data, u8* solid)
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
            int value = map_data[index];
            int z = solid[value]*2;
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

void drawLevelStart(GSGLOBAL* gsGlobal, HUD* hud, character* mario, int score, int lives)
{
    mario->x = 128;
    mario->y = 128;
    for(u16 tick = 0; tick < 128; tick++)
    {
        hud->draw(gsGlobal, 0, 0);
        hud->drawScore(gsGlobal, score);
        hud->drawWorld(gsGlobal, 1, 1);
        mario->draw(0, 0);
        hud->drawLives(gsGlobal, lives);
        gsKit_sync_flip(gsGlobal);
        gsKit_queue_exec(gsGlobal);
        gsKit_queue_reset(gsGlobal->Per_Queue);
        gsKit_clear(gsGlobal, 0);
    }
    printf("game started\n");
}

void drawStartScreen(GSGLOBAL* gsGlobal, controller* pad, HUD* hud, map* level1, u8* map_data, u8* solid)
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
        
        drawScreen(gsGlobal, &level1->spritesheet, 2, level1, start, map_data, solid);
        hud->drawDigit(gsGlobal, 250,250, 1);
        hud->drawString(gsGlobal, 282,250, "PLAYER GAME");
        hud->drawDigit(gsGlobal, 250,280, 2);
        hud->drawString(gsGlobal, 282,280, "PLAYER GAME");
        
        gsKit_sync_flip(gsGlobal);
        gsKit_queue_exec(gsGlobal);
        gsKit_queue_reset(gsGlobal->Per_Queue);
        gsKit_clear(gsGlobal, bg_color);
    }
}

void loadCoins(GSGLOBAL* gsGlobal, pickup* coin)
{
    for(int i = 0; i < 32; i++)
    {
        coin[i].spritesheet->Width = 64;
        coin[i].spritesheet->Height = 64;
        coin[i].spritesheet->PSM = GS_PSM_CT32;
        coin[i].width = 8;
        coin[i].height = 16;
    }

    coin[0].x = 260; coin[0].y = 144;
    coin[1].x = 372; coin[1].y = 144;
    coin[2].x = 356; coin[2].y = 80;
    coin[3].x = 1508; coin[3].y = 80;
    coin[4].x = 1700; coin[4].y = 144;
    coin[5].x = 1748; coin[5].y = 144;
    coin[6].x = 1796; coin[6].y = 144;
    coin[7].x = 2068; coin[7].y = 80;
    coin[8].x = 2084; coin[8].y = 80;
    coin[9].x = 2724; coin[9].y = 144;

    // underground coins
    coin[10].x = 2436; coin[10].y = 384; coin[10].activated = 1;
    coin[11].x = 2452; coin[11].y = 384; coin[11].activated = 1;
    coin[12].x = 2468; coin[12].y = 384; coin[12].activated = 1;
    coin[13].x = 2484; coin[13].y = 384; coin[13].activated = 1;
    coin[14].x = 2500; coin[14].y = 384; coin[14].activated = 1;
    coin[15].x = 2516; coin[15].y = 384; coin[15].activated = 1;
    coin[16].x = 2532; coin[16].y = 384; coin[16].activated = 1;
    coin[17].x = 2436; coin[17].y = 352; coin[17].activated = 1;
    coin[18].x = 2452; coin[18].y = 352; coin[18].activated = 1;
    coin[19].x = 2468; coin[19].y = 352; coin[19].activated = 1;
    coin[20].x = 2484; coin[20].y = 352; coin[20].activated = 1;
    coin[21].x = 2500; coin[21].y = 352; coin[21].activated = 1;
    coin[22].x = 2516; coin[22].y = 352; coin[22].activated = 1;
    coin[23].x = 2532; coin[23].y = 352; coin[23].activated = 1;
    
    coin[24].x = 2452; coin[24].y = 320; coin[24].activated = 1;
    coin[25].x = 2468; coin[25].y = 320; coin[25].activated = 1;
    coin[26].x = 2484; coin[26].y = 320; coin[26].activated = 1;
    coin[27].x = 2500; coin[27].y = 320; coin[27].activated = 1;
    coin[28].x = 2516; coin[28].y = 320; coin[28].activated = 1;
}

GSGLOBAL* character::gsGlobal;
GSGLOBAL* pickup::gsGlobal;

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

    HUD hud;
    hud.spritesheet.Width = 128;
    hud.spritesheet.Height = 128;
    hud.spritesheet.PSM = GS_PSM_CT32;
    gsKit_texture_abgr(gsGlobal, &hud.spritesheet, hud_array, hud.spritesheet.Width, hud.spritesheet.Height );
    
    map level1;
    level1.spritesheet.Width = 128;
    level1.spritesheet.Height = 128;
    level1.spritesheet.PSM = GS_PSM_CT32;
    gsKit_texture_abgr(gsGlobal, &level1.spritesheet, spritesheet_array, level1.spritesheet.Width, level1.spritesheet.Height );
    level1.data = map_data;

    character::gsGlobal = gsGlobal;
    
    character mario;
    mario.spritesheet.Width = 512;
    mario.spritesheet.Height = 32;
    mario.spritesheet.PSM = GS_PSM_CT32;
    gsKit_texture_abgr(gsGlobal, &mario.spritesheet, mario_array, mario.spritesheet.Width, mario.spritesheet.Height );

    character koopa;
    koopa.spritesheet.Width = 96;
    koopa.spritesheet.Height = 24;
    koopa.spritesheet.PSM = GS_PSM_CT32;
    gsKit_texture_abgr(gsGlobal, &koopa.spritesheet, koopa_array, koopa.spritesheet.Width, koopa.spritesheet.Height );
    koopa.height = 24;
    koopa.x = 1712;
    koopa.y = 184;

    pickup::gsGlobal = gsGlobal;
    
    pickup coin[32];
    loadCoins(gsGlobal, &coin[0]);
    gsKit_texture_abgr(gsGlobal, coin[0].spritesheet, pickups_array, coin[0].spritesheet->Width, coin[0].spritesheet->Height );
    for(int i = 1; i < 32; i++)coin[i].spritesheet = coin[0].spritesheet;

    
    pickup mushroom[4];
    for(int i = 0; i < 4; i++)
    {
        mushroom[i].spritesheet->Width = 64;
        mushroom[i].spritesheet->Height = 64;
        mushroom[i].spritesheet->PSM = GS_PSM_CT32;
        mushroom[i].width = 16;
        mushroom[i].height = 16;
        mushroom[i].type = 1;
        mushroom[i].activated = 0;
    }
    for(int i = 0; i < 4; i++)mushroom[i].spritesheet = coin[0].spritesheet;
    mushroom[0].x = 336; mushroom[0].y = 144;
    mushroom[1].x = 1024; mushroom[1].y = 128; mushroom[1].sprite = 1;

    pickup flower[2];
    for(int i = 0; i < 2; i++)
    {
        flower[i].spritesheet->Width = 64;
        flower[i].spritesheet->Height = 64;
        flower[i].spritesheet->PSM = GS_PSM_CT32;
        flower[i].width = 16;
        flower[i].height = 16;
        flower[i].type = 2;
    }
    for(int i = 0; i < 2; i++)flower[i].spritesheet = coin[0].spritesheet;
    flower[0].x = 1248; flower[0].y = 144;
    flower[1].x = 1744; flower[1].y = 80;
    
    character goomba[16];
    for(int i = 0; i < 16; i++)
    {
        goomba[i].spritesheet.Width = 32;
        goomba[i].spritesheet.Height = 16;
        goomba[i].spritesheet.PSM = GS_PSM_CT32;
        goomba[i].direction = 1;
    }
    gsKit_texture_abgr(gsGlobal, &goomba[0].spritesheet, goomba_array, goomba[0].spritesheet.Width, goomba[0].spritesheet.Height );
    for(int i = 1; i < 16; i++)goomba[i].spritesheet = goomba[0].spritesheet;
    goomba[0].x = 352; goomba[0].y = 192;goomba[0].direction = 0;
    goomba[1].x = 640; goomba[1].y = 192;
    goomba[2].x = 816; goomba[2].y = 192;
    goomba[3].x = 848; goomba[3].y = 192;
    goomba[4].x = 1280; goomba[4].y = 64;
    goomba[5].x = 1312; goomba[5].y = 64;
    goomba[6].x = 1552; goomba[6].y = 192;
    goomba[7].x = 1576; goomba[7].y = 192;
    goomba[8].x = 1824; goomba[8].y = 192;
    goomba[9].x = 1848; goomba[9].y = 192;
    goomba[10].x = 1984; goomba[10].y = 192;
    goomba[11].x = 2008; goomba[11].y = 192;
    goomba[12].x = 2048; goomba[12].y = 192;
    goomba[13].x = 2072; goomba[13].y = 192;
    goomba[14].x = 2784; goomba[14].y = 192;
    goomba[15].x = 2808; goomba[15].y = 192;
    
    block* box = NULL;
    
    u8 solid[64] = {0,1,0,0,0,0,0,0,
                    0,0,0,1,1,1,0,0,
                    1,1,0,0,0,0,0,0,
                    1,1,0,0,0,0,0,0,
                    1,1,1,1,0,0,0,0,
                    1,1,1,0,0,0,0,0,
                    1,1,1,0,0,0,0,0,
                    0,0,0,0,0,0,0,0};
    
    mario.vy = 0;
    int gravity = 1;
    u8 tick = 0;
    u8 superMario = 0;
    int time = 400;
    int score = 0;
    int lives = 3;
    u8 frameByFrame = 0;

    drawStartScreen(gsGlobal, & pad, &hud, &level1, map_data, solid);
    
    drawLevelStart(gsGlobal, &hud, &mario, score, lives);
    mario.x = 0;
    mario.y = 192;
    
    while(1)
    {
        // set the background color
        if(viewport.y == 0)bg_color = GS_SETREG_RGBAQ(0x5C,0x94,0xFC,0x00,0x00); // above ground
        else bg_color = GS_SETREG_RGBAQ(0x00,0x00,0x00,0x00,0x00); // underground (in cave)
        gsKit_clear(gsGlobal, bg_color);

        pad.read();
        if(!mario.canMoveDown(&level1, solid, 1) && !mario.animationMode)mario.sprite = 0;
        else if(!mario.animationMode) mario.sprite = 5;
        // mario fell into a pit
        if(!mario.animationMode && mario.y > viewport.y + 208)
        {
            printf("fell\n");
            mario.animationMode = 4;
        }
        if(mario.pipeOnRight(&level1))mario.animationMode = 5;

        if(!mario.animationMode)
        {
            mario.reactToControllerInput(&pad, tick, &level1, solid, &viewport.x, &viewport.y, scale_factor, &superMario, &frameByFrame);
        }
        // mario is entering a pipe
        else if(mario.animationMode == 1)
        {
            if(mario.animationFrame < 10)
            {
                if((tick & 7) == 0)
                {
                    mario.y+=2;
                    mario.animationFrame++;
                }
            }
            else
            {
                mario.animationMode = 0;
                mario.animationFrame = 0;
                //mario.x = 0; // respawn mario
                //x = 0; // re-position camera
                viewport.x = 2368;
                viewport.y = 240;
                mario.x = 2384;
                mario.y = 240;
            }
        }
        // growing into big mario
        else if(mario.animationMode == 2)
        {
            u8 arr1[10] = {0, 13, 0, 13, 0, 13, 15, 0, 13, 15};
            char arr2[10] = {0, -16, 16,-16,16,-16,0,16,-16,0};
            
            if(mario.animationFrame < 10)
            {
                if((tick & 3) == 0)
                {
                    mario.sprite = arr1[mario.animationFrame];
                    mario.y += arr2[mario.animationFrame];
                    if(mario.sprite == 0)
                    {
                        mario.height = 16;
                        mario.width = 16;
                    }
                    else
                    {
                        mario.height = 32;
                        mario.width = 18;
                        
                    }
                    mario.animationFrame++;
                }
            }
            else
            {
                mario.animationMode = 0;
                mario.animationFrame = 0;
                mario.sprite = 0;
                superMario = 1;
            }
        }
        // shrinking back down to small mario
        else if(mario.animationMode == 3)
        {
            
        }
        // death animation
        else if(mario.animationMode == 4)
        {
            //u8 arr[10] = {10,10,10,10,10,-20,-20,-20,-20,-20};
            mario.sprite = 13;
            mario.collisionDetection = 0;
            if(mario.animationFrame < 15)
            {
                if((tick & 1) == 0)
                {
                    mario.y -= 5;
                    mario.animationFrame++;
                }
            }
            else if(mario.animationFrame < 80)
            {
                if((tick & 1) == 0)
                {
                    mario.y += 5;
                    mario.animationFrame++;
                }
            }
            else
            {
                mario.animationMode = 0;
                mario.animationFrame = 0;
                mario.sprite = 0;
                drawLevelStart(gsGlobal, &hud, &mario, score, lives);
                mario.x = 0;
                viewport.x = 0;
                mario.y = 192;
                mario.collisionDetection = 1;
            }
        }
        else if(mario.animationMode == 5)
        {
            mario.vy = 0;
            if(mario.animationFrame < 10)
            {
                if((tick & 7) == 0)
                {
                    mario.x+=2;
                    mario.animationFrame++;
                }
            }
            else
            {
                mario.animationMode = 0;
                mario.animationFrame = 0;
                viewport.x = 0;
                viewport.y = 0;
                mario.x = 0;
                mario.y = 192;
            }
        }
        
        

        // dealing with gravity
        if(mario.vy > 0)
        {
            // did mario stomp any goombas?
            for(int i = 0; i < 16; i++)
            {
                if(goomba[i].isOnScreen(viewport.x) && mario.isTouching(&goomba[i]))
                {
                    goomba[i].sprite = 1;
                    mario.vy = -6;
                    goomba[i].activated = 0;
                    goomba[i].collisionDetection = 0;
                }
            }
            
            if(!mario.animationMode && mario.canMoveDown(&level1, solid, mario.vy))
            {
                mario.y += mario.vy;
                if((tick & 3) == 0)mario.vy += gravity;
            }
            else if(!mario.animationMode)
            {
                while(mario.vy > 0 && !mario.canMoveDown(&level1, solid, mario.vy))mario.vy--;
                mario.y += mario.vy;
                mario.vy = 0;
                
                mario.sprite = 0;
            }
        }
        else if(mario.vy < 0 || mario.canMoveDown(&level1, solid, 1)) // jumping
        {
            if(mario.canMoveUp(&level1, solid, mario.vy*(-1)))
            {
                mario.y += mario.vy;
                if((tick & 3) == 0)mario.vy += gravity;
            }
            else
            {
                while(mario.vy < 0 && !mario.canMoveUp(&level1, solid, mario.vy*(-1)))mario.vy++;
                if(mario.vy == 0)
                {
                    int index = level1.get_index(mario.x + 8, mario.y - 1);
                    int coinx = ((index % 224) * 16)+4;
                    int coiny = (index / 224) * 16;
                    printf("hit box %d (%d) coin location: <%d, %d>\n", index,level1.data[index], coinx, coiny);
                    if(level1.data[index] == 11 || level1.data[index] == 33)
                    {
                        block block1;
                        block1.sprite = 32;
                        block1.x = ((mario.x + 8) >> 4)<<4;
                        block1.y = ((mario.y - 1) >> 4)<<4;
                        block1.spritesheet = level1.spritesheet;
                        level1.data[index] = 0;
                        box = &block1;
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
                mario.y += mario.vy;
            }
        }
        else if(mario.vy == 0) // mario is only able to get killed by enemies when not jumping or falling
        {
            for(int i = 0; i < 16; i++)
            {
                if(mario.animationMode == 0 && goomba[i].isOnScreen(viewport.x) && mario.isTouching(&goomba[i]))
                {
                    printf("touching a goomba\n");
                    //mario.x = 0;
                    //x = 0;
                    //mario.y = 192;
                    mario.animationMode = 4;
                    //drawLevelStart(gsGlobal, &hud, &mario, score, lives);
                }
            }
        }
        if(superMario)mario.sprite+=15;
        drawScreen(gsGlobal, &level1.spritesheet, scale_factor, &level1, viewport, map_data, solid);
        mario.draw(viewport.x, viewport.y);
        for(int i = 0; i < 32; i++)
        {
            coin[i].draw(gsGlobal, viewport.x, viewport.y);
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
            mushroom[i].draw(gsGlobal, viewport.x, viewport.y);
            mushroom[i].update();
        }
        for(int i = 0; i < 2; i++)
        {
            flower[i].draw(gsGlobal, viewport.x, viewport.y);
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
            if(goomba[i].isOnScreen(viewport.x))goomba[i].draw(viewport.x, viewport.y);
        }
        if(koopa.isOnScreen(viewport.x))koopa.draw(viewport.x, viewport.y);

        if(box)
        {
            box->draw(gsGlobal, viewport.x, viewport.y);
            if(tick&1)
            {
                if(box->update())
                {
                    int index_x = box->x / level1.tile_width;
                    int index_y = box->y / level1.tile_height;
                    level1.data[(index_y*level1.width)+index_x] = 32;
                }
            }
        }
        hud.draw(gsGlobal, 0, 0);
        hud.drawTime(gsGlobal, time);
        hud.drawScore(gsGlobal, score);
        hud.drawWorld(gsGlobal, 1, 1);
        
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
                    if(goomba[i].x > viewport.x && goomba[i].x < viewport.x + 320)goomba[i].hflip^=1;
                }
                for(int i = 0; i < 32; i++)
                {
                    coin[i].sprite++;
                    coin[i].sprite&=3;
                }
            }
            for(int i = 0; i < 16; i++)
            {
                printf("%d: ", i);
                goomba[i].print();
                if(goomba[i].x > viewport.x && goomba[i].x < viewport.x + 320)
                {
                    goomba[i].traverse(&level1, solid);
                    goomba[i].gravity(&level1, solid, tick, gravity);
                }
            }
            koopa.traverse(&level1, solid);
            koopa.gravity(&level1, solid, tick, gravity);
            koopa.hflip = koopa.direction ^ 1;
            if((tick&7) == 0)koopa.sprite = 2;
            else koopa.sprite = 3;
            
        }
        if((tick&31) == 0)time--; // every 32 frames
        tick++;
    }
    return 0;
}

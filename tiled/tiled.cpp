//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>

#include "spritesheet.h"
#include "mario.h"
#include "controller.hpp"
#include "map.hpp"
#include "map_data.h"
#include "character.hpp"

void gsKit_texture_abgr(GSGLOBAL* gsGlobal, GSTEXTURE* tex, u32* arr, u32 width, u32 height)
{
    u32 VramTextureSize = gsKit_texture_size(width, height, GS_PSM_CT32);

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

void drawTile(GSGLOBAL* gsGlobal, GSTEXTURE* spritesheet, int scale_factor, map* level, int start_x, int start_y, int value)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);// set color
    int index_x = value % 8; // 8 is the width of the tilesheet, in tiles
    int index_y = value / 8; // 8 is the width of the tilesheet, in tiles
                
    gsKit_prim_quad_texture(gsGlobal, spritesheet,
                            start_x*scale_factor, start_y*scale_factor,                                // x1, y1
                            level->tile_width*index_x, level->tile_height*index_y,         // u1, v1
                                    
                            start_x*scale_factor, (start_y + level->tile_height)*scale_factor,                  // x2, y2
                            level->tile_width*index_x, level->tile_height*(index_y+1),     // u2, y2

                            (start_x + level->tile_width)*scale_factor, start_y*scale_factor,                   // x3, y3
                            level->tile_width*(index_x+1), level->tile_height*index_y,     // u3, v3
                            
                            (start_x + level->tile_width)*scale_factor, (start_y + level->tile_height)*scale_factor,     // x4, y4
                            level->tile_width*(index_x+1), level->tile_height*(index_y+1), // u4, v4
                            0,TexCol);
}

void drawScreen(GSGLOBAL* gsGlobal, GSTEXTURE* spritesheet, int scale_factor, map* level, int x, int y, u8* map_data)
{
    // figure out the tile to draw
    int tile_x = x / level->tile_width;
    int tile_y = y / level->tile_height;

    // where do we start in the tile
    int point_x = x % level->tile_width;
    int point_y = y % level->tile_height;

    // position of the top left corner of the current tile
    int start_x = 0 - point_x;
    int start_y = 0 - point_y;
    
    while(start_y < gsGlobal->Height && tile_y < level->height)
    {
        while(start_x < gsGlobal->Width && tile_x < level->width)
        {
            int index = tile_y * level->width + tile_x;
            int value = map_data[index];
            drawTile(gsGlobal, spritesheet, scale_factor, level, start_x, start_y, value);
            start_x += level->tile_width;
            tile_x++;
        }
        start_x = 0 - point_x;
        tile_x = x / level->tile_width;
        start_y += level->tile_height;
        tile_y++;
    }
}

int main()
{   
    GSGLOBAL* gsGlobal = gsKit_init_global();
    gsGlobal->Mode = GS_MODE_PAL;
    gsGlobal->Width=640;
    gsGlobal->Height=512;
    gsGlobal->PSMZ = GS_PSMZ_32;
    gsGlobal->PSM = GS_PSM_CT32;
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
    int x = 0, y=0; // top left corner of the screen
    int scale_factor = 2; 
    u64 bg_color = GS_SETREG_RGBAQ(0x5C,0x94,0xFC,0x00,0x00);
    
    map level1;
    level1.spritesheet.Width = 128;
    level1.spritesheet.Height = 64;
    level1.spritesheet.PSM = GS_PSM_CT32;
    gsKit_texture_abgr(gsGlobal, &level1.spritesheet, spritesheet_array, level1.spritesheet.Width, level1.spritesheet.Height );
    level1.data = map_data;
    
    character mario;
    mario.spritesheet.Width = 256;
    mario.spritesheet.Height = 16;
    mario.spritesheet.PSM = GS_PSM_CT32;
    gsKit_texture_abgr(gsGlobal, &mario.spritesheet, mario_array, mario.spritesheet.Width, mario.spritesheet.Height );

    u8 solid[32] = {0,1,0,0,0,0,0,0,
                    0,0,0,1,1,1,0,0,
                    1,1,0,0,0,0,0,0,
                    1,1,0,0,0,0,0,0};
    mario.vy = 0;
    int gravity = 1;
    u8 tick = 0;
    while(1)
    {
        gsKit_clear(gsGlobal, bg_color);
        pad.read();
        if(!mario.canMoveDown(&level1, solid,scale_factor, x, y, 1))mario.sprite = 0;
        else mario.sprite = 5;
        
        if(pad.left())
        {
            mario.hflip = 1;
            if(tick&1)mario.gate++;
            mario.gate &= 3;
            if(!mario.canMoveDown(&level1, solid,scale_factor, x, y, 2))
            {
                if(mario.gate & 1)mario.sprite = 2;
                else
                {
                    if(!mario.gate) mario.sprite = 1;
                    else mario.sprite = 3;
                }
            }
            if(mario.x > (gsGlobal->Width/2))
            {
                if(mario.canMoveLeft(&level1, solid,scale_factor, x, y, 2))mario.x-=4;
            }
            else if(x > 0)
            {
                if(mario.canMoveLeft(&level1, solid,scale_factor, x, y, 2))x-=2;
            }
            else if(mario.x > 0)
            {
                if(mario.canMoveLeft(&level1, solid,scale_factor, x, y, 2))mario.x-=4;
            }
        }
        if(pad.right())
        {
            mario.hflip = 0;
            if(tick)mario.gate++;
            mario.gate &= 3;
            if(!mario.canMoveDown(&level1, solid,scale_factor, x, y, 2))
            {
                if(mario.gate & 1)mario.sprite = 2;
                else
                {
                    if(!mario.gate) mario.sprite = 1;
                    else mario.sprite = 3;
                }
            }
            if(mario.x < (gsGlobal->Width/2))
            {
                if(mario.canMoveRight(&level1, solid,scale_factor, x, y, 2))mario.x+=4;
            }
            else if(x+(gsGlobal->Width/scale_factor) < (level1.width*level1.tile_width))
            {
                if(mario.canMoveRight(&level1, solid,scale_factor, x, y, 2))x+=2;
            }
            else if(mario.x+(16*scale_factor) < (level1.width*level1.tile_width))
            {
                if(mario.canMoveRight(&level1, solid,scale_factor, x, y, 2))mario.x+=4;
            }
        }
        if(pad.up() || pad.x())
        {
            mario.sprite = 5;
            if(mario.y > 0)
            {
                if(mario.canMoveUp(&level1, solid,scale_factor, x, y, 2) &&
                   !mario.canMoveDown(&level1, solid,scale_factor, x, y, 2))mario.vy = -16;//mario.y-=2;
            }
        }
        if(pad.down()) //redundant thanks to gravity
        {
            if(mario.y+(16*scale_factor) < level1.height*level1.tile_height*scale_factor)
            {
                if(mario.canMoveDown(&level1, solid,scale_factor, x, y, 2))mario.y+=2;
            }
        }

        // dealing with gravity
        if(mario.vy > 0)
        {
            if(mario.canMoveDown(&level1, solid,scale_factor, x, y, mario.vy))
            {
                mario.y += mario.vy;
                mario.vy += gravity;
            }
            else
            {
                while(mario.vy > 0 && !mario.canMoveDown(&level1, solid,scale_factor, x, y, mario.vy))mario.vy--;
                mario.y += mario.vy;
                mario.vy = 0;
                mario.sprite = 0;
            }
        }
        else if(mario.vy < 0 || mario.canMoveDown(&level1, solid,scale_factor, x, y, 1)) // jumping
        {
            //printf("jumping\n");
            if(mario.canMoveUp(&level1, solid,scale_factor, x, y, mario.vy*(-1)))
            {
                //printf("jumping %d\n", mario.vy);
                mario.y += mario.vy;
                mario.vy += gravity;
            }
            else
            {
                while(mario.vy < 0 && !mario.canMoveUp(&level1, solid,scale_factor, x, y, mario.vy*(-1)))mario.vy++;
                if(mario.vy == 0)
                {
                    int index = level1.get_index(mario.x/scale_factor + x + 8, mario.y/scale_factor + y - 1);
                    printf("hit box %d\n", level1.data[index]);
                    if(level1.data[index] == 11)level1.data[index]++;
                }
                mario.y += mario.vy;
            }
        }
        
        drawScreen(gsGlobal, &level1.spritesheet, scale_factor, &level1, x, y, map_data);
        mario.draw(gsGlobal);
        gsKit_sync_flip(gsGlobal);
        gsKit_queue_exec(gsGlobal);
        gsKit_queue_reset(gsGlobal->Per_Queue);
        gsKit_clear(gsGlobal, 0);
        tick++;
        tick&=15;
    }
    return 0;
}

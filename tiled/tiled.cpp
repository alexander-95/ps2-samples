//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>

#include "spritesheet.h"
#include "mario.h"
#include "goomba.h"
#include "koopa.h"
#include "controller.hpp"
#include "map.hpp"
#include "map_data.h"
#include "character.hpp"
#include "block.hpp"

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

void drawTile(GSGLOBAL* gsGlobal, GSTEXTURE* spritesheet, int scale_factor, map* level, int start_x, int start_y, int value, int z)
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
                            z,TexCol);
}

void drawScreen(GSGLOBAL* gsGlobal, GSTEXTURE* spritesheet, int scale_factor, map* level, int x, int y, u8* map_data, u8* solid)
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
            int z = solid[value]*2;
            drawTile(gsGlobal, spritesheet, scale_factor, level, start_x, start_y, value, z);
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
    
    character goomba[16];
    for(int i = 0; i < 16; i++)
    {
        goomba[i].spritesheet.Width = 32;
        goomba[i].spritesheet.Height = 16;
        goomba[i].spritesheet.PSM = GS_PSM_CT32;
    }
    gsKit_texture_abgr(gsGlobal, &goomba[0].spritesheet, goomba_array, goomba[0].spritesheet.Width, goomba[0].spritesheet.Height );
    goomba[0].x = 352;
    goomba[0].y = 192;
    goomba[0].direction = 0;

    goomba[1].x = 640;
    goomba[1].y = 192;
    goomba[1].direction = 1;
    goomba[1].spritesheet = goomba[0].spritesheet;

    goomba[2].x = 816;
    goomba[2].y = 192;
    goomba[2].direction = 1;
    goomba[2].spritesheet = goomba[0].spritesheet;

    goomba[3].x = 848;
    goomba[3].y = 192;
    goomba[3].direction = 1;
    goomba[3].spritesheet = goomba[0].spritesheet;

    goomba[4].x = 1280;
    goomba[4].y = 64;
    goomba[4].direction = 1;
    goomba[4].spritesheet = goomba[0].spritesheet;

    goomba[5].x = 1312;
    goomba[5].y = 64;
    goomba[5].direction = 1;
    goomba[5].spritesheet = goomba[0].spritesheet;

    goomba[6].x = 1552;
    goomba[6].y = 192;
    goomba[6].direction = 1;
    goomba[6].spritesheet = goomba[0].spritesheet;

    goomba[7].x = 1576;
    goomba[7].y = 192;
    goomba[7].direction = 1;
    goomba[7].spritesheet = goomba[0].spritesheet;

    goomba[8].x = 1824;
    goomba[8].y = 192;
    goomba[8].direction = 1;
    goomba[8].spritesheet = goomba[0].spritesheet;
    
    goomba[9].x = 1848;
    goomba[9].y = 192;
    goomba[9].direction = 1;
    goomba[9].spritesheet = goomba[0].spritesheet;

    goomba[10].x = 1984;
    goomba[10].y = 192;
    goomba[10].direction = 1;
    goomba[10].spritesheet = goomba[0].spritesheet;

    goomba[11].x = 2008;
    goomba[11].y = 192;
    goomba[11].direction = 1;
    goomba[11].spritesheet = goomba[0].spritesheet;

    goomba[12].x = 2048;
    goomba[12].y = 192;
    goomba[12].direction = 1;
    goomba[12].spritesheet = goomba[0].spritesheet;

    goomba[13].x = 2072;
    goomba[13].y = 192;
    goomba[13].direction = 1;
    goomba[13].spritesheet = goomba[0].spritesheet;

    goomba[14].x = 2784;
    goomba[14].y = 192;
    goomba[14].direction = 1;
    goomba[14].spritesheet = goomba[0].spritesheet;

    goomba[15].x = 2808;
    goomba[15].y = 192;
    goomba[15].direction = 1;
    goomba[15].spritesheet = goomba[0].spritesheet;
    
    block* box = NULL;
    
    u8 solid[32] = {0,1,0,0,0,0,0,0,
                    0,0,0,1,1,1,0,0,
                    1,1,0,0,0,0,0,0,
                    1,1,0,0,0,0,0,0};
    mario.vy = 0;
    int gravity = 1;
    u8 tick = 0;
    u8 superMario = 0;
    while(1)
    {
        gsKit_clear(gsGlobal, bg_color);
        pad.read();
        if(!mario.canMoveDown(&level1, solid, 1))mario.sprite = 0;
        else mario.sprite = 5;

        if(!mario.animationMode)
        {
            if(pad.left())
            {
                mario.hflip = 1;
                if(tick&1)mario.gate++;
                mario.gate &= 3;
                if(!mario.canMoveDown(&level1, solid, 2))
                {
                    if(mario.gate & 1)mario.sprite = 2;
                    else
                    {
                        if(!mario.gate) mario.sprite = 1;
                        else mario.sprite = 3;
                    }
                }
                if(mario.canMoveLeft(&level1, solid, 2))
                {
                    mario.x-=2;
                    if(x > 0)x-=2;
                }
            }
            if(pad.right())
            {
                mario.hflip = 0;
                if(tick&1)mario.gate++;
                mario.gate &= 3;
                if(!mario.canMoveDown(&level1, solid, 2))
                {
                    if(mario.gate & 1)mario.sprite = 2;
                    else
                    {
                        if(!mario.gate) mario.sprite = 1;
                        else mario.sprite = 3;
                    }
                }
                if(mario.canMoveRight(&level1, solid, 2))
                {
                    mario.x+=2;
                    if(x+(gsGlobal->Width/(2*scale_factor)) < (level1.width*level1.tile_width) &&
                       mario.x > (gsGlobal->Width/(2*scale_factor))) x+=2;
                }
            }
            if(pad.up() || pad.x())
            {
                mario.sprite = 5;
                if(mario.y > 0)
                {
                    if(mario.canMoveUp(&level1, solid, 2) &&
                       !mario.canMoveDown(&level1, solid, 2))mario.vy = -6;//mario.y-=2;
                }
            }
            if(pad.down())
            {
                if(mario.standingOnPipe(&level1))
                {
                    printf("standing on pipe\n");
                    mario.animationMode = 1;
                }
            }
            if(pad.triangle() && (tick & 8))
            {
                if(superMario)
                {
                    superMario = 0;
                    mario.height = 16;
                    mario.width = 16;
                    mario.y+=16;
                }
                else
                {
                    superMario = 1;
                    mario.height = 32;
                    mario.width = 18;
                    mario.y -= 16;
                }
            }
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
                mario.x = 0; // respawn mario
                x = 0; // re-position camera
            }
        }
        // mario is dying
        else if(mario.animationMode == 2)
        {
            printf("dead!\n");
        }

        // dealing with gravity
        if(mario.vy > 0)
        {
            // did mario stomp any goombas?
            for(int i = 0; i < 16; i++)
            {
                if(goomba[i].isOnScreen(x) && mario.isTouching(&goomba[i]))
                {
                    goomba[i].sprite = 1;
                    mario.vy = -6;
                }
            }
            
            if(mario.canMoveDown(&level1, solid, mario.vy))
            {
                mario.y += mario.vy;
                if((tick & 3) == 0)mario.vy += gravity;
            }
            else
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
                    printf("hit box %d\n", level1.data[index]);
                    if(level1.data[index] == 11)
                    {
                        block block1;
                        block1.sprite = 1;
                        block1.x = ((mario.x + 8) >> 4)<<4;
                        block1.y = ((mario.y - 1) >> 4)<<4;
                        block1.spritesheet = level1.spritesheet;
                        level1.data[index] = 0;
                        box = &block1;
                    }
                }
                mario.y += mario.vy;
            }
        }
        if(superMario)mario.sprite+=15;
        drawScreen(gsGlobal, &level1.spritesheet, scale_factor, &level1, x, y, map_data, solid);
        mario.draw(gsGlobal, x, y);
        for(int i = 0;i < 16; i++)
        {
            if(goomba[i].isOnScreen(x))goomba[i].draw(gsGlobal, x, y);
        }
        if(koopa.isOnScreen(x))koopa.draw(gsGlobal, x, y);

        if(box)
        {
            box->draw(gsGlobal, x, y);
            if(tick&1)
            {
                if(box->update())
                {
                    int index_x = box->x / level1.tile_width;
                    int index_y = box->y / level1.tile_height;
                    level1.data[(index_y*level1.width)+index_x] = 1;
                }
            }
        }
        
        gsKit_sync_flip(gsGlobal);
        gsKit_queue_exec(gsGlobal);
        gsKit_queue_reset(gsGlobal->Per_Queue);
        gsKit_clear(gsGlobal, 0);
        if((tick&3) == 0)
        {
            if((tick&7) == 0)
            {
                for(int i = 0; i < 16; i++)
                {
                    if(goomba[i].x > x && goomba[i].x < x + 320)goomba[i].hflip^=1;
                }
            }
            for(int i = 0; i < 16; i++)
            {
                if(goomba[i].x > x && goomba[i].x < x + 320)
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
        tick++;
        tick&=15;
    }
    return 0;
}

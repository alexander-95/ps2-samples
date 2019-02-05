//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>

#include "spritesheet.h"
#include "mario.h"
#include "goomba.h"
#include "koopa.h"
#include "pickups.h"

#include "controller.hpp"
#include "map.hpp"
#include "map_data.h"
#include "character.hpp"
#include "block.hpp"
#include "pickup.hpp"

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

    u8 rowsDrawn = 0;

    while(start_y < gsGlobal->Height && tile_y < level->absoluteHeight && rowsDrawn < level->height)
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
        rowsDrawn++;
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
    gsGlobal->PSM = GS_PSM_CT16;
    gsGlobal->ZBuffering = GS_SETTING_OFF;
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
    level1.spritesheet.Height = 128;
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

    pickup coin[32];
    for(int i = 0; i < 32; i++)
    {
        coin[i].spritesheet.Width = 64;
        coin[i].spritesheet.Height = 64;
        coin[i].spritesheet.PSM = GS_PSM_CT32;
        coin[i].width = 8;
        coin[i].height = 16;
    }
    gsKit_texture_abgr(gsGlobal, &coin[0].spritesheet, pickups_array, coin[0].spritesheet.Width, coin[0].spritesheet.Height );
    for(int i = 1; i < 32; i++)coin[i].spritesheet = coin[0].spritesheet;
    coin[0].x = 260; coin[0].y = 144;
    coin[1].x = 372; coin[1].y = 144;
    coin[2].x = 356; coin[2].y = 80;
    coin[17].x = 1508; coin[17].y = 80;
    coin[18].x = 1700; coin[18].y = 144;
    coin[19].x = 1748; coin[19].y = 144;
    coin[20].x = 1796; coin[20].y = 144;
    coin[21].x = 2068; coin[21].y = 80;
    coin[22].x = 2084; coin[22].y = 80;
    coin[23].x = 2724; coin[23].y = 144;

    pickup mushroom[4];
    for(int i = 0; i < 4; i++)
    {
        mushroom[i].spritesheet.Width = 64;
        mushroom[i].spritesheet.Height = 64;
        mushroom[i].spritesheet.PSM = GS_PSM_CT32;
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
        flower[i].spritesheet.Width = 64;
        flower[i].spritesheet.Height = 64;
        flower[i].spritesheet.PSM = GS_PSM_CT32;
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
                    1,1,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0};
    
    mario.vy = 0;
    int gravity = 1;
    u8 tick = 0;
    u8 superMario = 0;
    while(1)
    {
        
        gsKit_clear(gsGlobal, bg_color);
        pad.read();
        if(!mario.canMoveDown(&level1, solid, 1) && !mario.animationMode)mario.sprite = 0;
        else if(!mario.animationMode) mario.sprite = 5;
        // mario fell into a pit
        if(false && mario.y > 208)
        {
            printf("fell\n");
            mario.x = 0;
            x = 0;
            mario.y = 192;
        }

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
            if(pad.up() || pad.x(1))
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
            if(pad.triangle(1))
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
                //mario.x = 0; // respawn mario
                //x = 0; // re-position camera
                x = 2368;
                y = 240;
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

        // dealing with gravity
        if(mario.vy > 0 && !mario.animationMode)
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
                        if(index == 2032)coin[0].activated = 1;
                        else if(index == 2037)mushroom[0].activated = 1;
                        else if(index == 2039)coin[1].activated = 1;
                        else if(index == 1142)coin[2].activated = 1;
                        else if(index == 1214)coin[17].activated = 1;
                        else if(index == 2122)coin[18].activated = 1;
                        else if(index == 2125)coin[19].activated = 1;
                        else if(index == 2128)coin[20].activated = 1;
                        else if(index == 1249)coin[21].activated = 1;
                        else if(index == 1250)coin[22].activated = 1;
                        else if(index == 2186)coin[23].activated = 1;
                        else if(index == 2094)flower[0].activated = 1;
                        else if(index == 1229)flower[1].activated = 1;
                        else if(index == 1856)mushroom[1].activated = 1;
                    }
                }
                mario.y += mario.vy;
            }
        }
        if(superMario)mario.sprite+=15;
        drawScreen(gsGlobal, &level1.spritesheet, scale_factor, &level1, x, y, map_data, solid);
        mario.draw(gsGlobal, x, y);
        for(int i = 0; i < 32; i++)
        {
            coin[i].draw(gsGlobal, x, y);
            coin[i].update();
        }
        for(int i = 0; i < 4; i++)
        {
            if(mario.pickedup(&mushroom[0]))
            {
                printf("picked up mushroom");
                mushroom[i].activated = 0;
                mario.animationMode = 2;
            }
            mushroom[i].draw(gsGlobal, x, y);
            mushroom[i].update();
        }
        for(int i = 0; i < 2; i++)
        {
            flower[i].draw(gsGlobal, x, y);
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
                    level1.data[(index_y*level1.width)+index_x] = 32;
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
                for(int i = 0; i < 32; i++)
                {
                    coin[i].sprite++;
                    coin[i].sprite&=3;
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
        tick&=31;
    }
    return 0;
}

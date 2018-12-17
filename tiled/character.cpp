//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <stdio.h>
#include "character.hpp"

character::character()
{
    x = 0;
    y = 0;
    sprite = 0;
}

character::~character()
{
    
}

void character::draw(GSGLOBAL* gsGlobal)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);// set color
    gsKit_prim_quad_texture(gsGlobal, &spritesheet,
                            x,y,         // x1, y1
                            16*sprite, 0,         // u1, v1

                            x,y+32,         // x2, y2
                            16*sprite, 16,         // u2, v2

                            x+32,y,         // x3, y3
                            16*(sprite+1), 0,         // u3, v3

                            x+32,y+32,         // x4, y4
                            16*(sprite+1), 16,         // u4, v4
                            1, TexCol);
}

int character::canMoveDown(map* level, u8* map_data, u8* solid, int scale_factor, int screen_x, int screen_y )
{
    int x1 = x+screen_x*scale_factor, y1 = y+32+screen_y*scale_factor, x2 = x+30+screen_x*scale_factor, y2 = y+32+screen_y*scale_factor;
    // figure out which tile mario is running into
    int tile_x1 = (x1 / level->tile_width) / scale_factor;
    int tile_y1 = (y1 / level->tile_height) / scale_factor;
    int index1 = tile_y1 * level->width + tile_x1;
    int value1 = map_data[index1];
                
    int tile_x2 = (x2 / level->tile_width) / scale_factor;
    int tile_y2 = (y2 / level->tile_height) / scale_factor;
    int index2 = tile_y2 * level->width + tile_x2;
    int value2 = map_data[index2];

    //printf("screen: %d, %d\n", screen_x, screen_y);

    //printf("mario's x: %d\n", x1);
    //printf("tile %d, %d\n", tile_x1, tile_y1);
    //printf("blocks below mario: %d, %d\n", value1, value2);

    if(!solid[value1] && !solid[value2]) return 1;
    else return 0;
}

int character::canMoveUp(map* level, u8* map_data, u8* solid, int scale_factor, int screen_x, int screen_y )
{
    int x1 = x+screen_x*scale_factor, y1 = (y-2)+screen_y*scale_factor, x2 = x+30+screen_x*scale_factor, y2 = (y-2)+screen_y*scale_factor;
    // figure out which tile mario is running into
    int tile_x1 = (x1 / level->tile_width) / scale_factor;
    int tile_y1 = (y1 / level->tile_height) / scale_factor;
    int index1 = tile_y1 * level->width + tile_x1;
    int value1 = map_data[index1];

    int tile_x2 = (x2 / level->tile_width) / scale_factor;
    int tile_y2 = (y2 / level->tile_height) / scale_factor;
    int index2 = tile_y2 * level->width + tile_x2;
    int value2 = map_data[index2];

    if(!solid[value1] && !solid[value2]) return 1;
    else return 0;
}

int character::canMoveRight(map* level, u8* map_data, u8* solid, int scale_factor, int screen_x, int screen_y )
{
    int x1 = (x+32)+screen_x*scale_factor, y1 = y+screen_y*scale_factor, x2 = (x+32)+screen_x*scale_factor, y2 = (y+30)+screen_y*scale_factor;
    // figure out which tile mario is running into
    int tile_x1 = (x1 / level->tile_width) / scale_factor;
    int tile_y1 = (y1 / level->tile_height) / scale_factor;
    int index1 = tile_y1 * level->width + tile_x1;
    int value1 = map_data[index1];

    int tile_x2 = (x2 / level->tile_width) / scale_factor;
    int tile_y2 = (y2 / level->tile_height) / scale_factor;
    int index2 = tile_y2 * level->width + tile_x2;
    int value2 = map_data[index2];

    if(!solid[value1] && !solid[value2]) return 1;
    else return 0;
}

int character::canMoveLeft(map* level, u8* map_data, u8* solid, int scale_factor, int screen_x, int screen_y )
{
    int x1 = (x-2)+screen_x*scale_factor, y1 = y+screen_y*scale_factor, x2 = (x-2)+screen_x*scale_factor, y2 = (y+30)+screen_y*scale_factor;
    // figure out which tile mario is running into
    int tile_x1 = (x1 / level->tile_width) / scale_factor;
    int tile_y1 = (y1 / level->tile_height) / scale_factor;
    int index1 = tile_y1 * level->width + tile_x1;
    int value1 = map_data[index1];

    int tile_x2 = (x2 / level->tile_width) / scale_factor;
    int tile_y2 = (y2 / level->tile_height) / scale_factor;
    int index2 = tile_y2 * level->width + tile_x2;
    int value2 = map_data[index2];

    if(!solid[value1] && !solid[value2]) return 1;
    else return 0;
}

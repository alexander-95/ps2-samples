//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <stdio.h>
#include "block.hpp"

block::block()
{
    x = 0;
    y = 0;
    sprite = 0;
    phase = 0;
}

block::~block()
{
    printf("block destructor\n");
}

void block::draw()
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);// set color
    u8 spritex = sprite % 8; // 8 is the width of the tilesheet in tiles
    u8 spritey = sprite / 8;
    int u1 = 16*spritex, v1 = 16*spritey, u2 = 16*(spritex+1), v2 = 16*(spritey+1);
    int x1 = (x-viewport->x)*2, y1 = (y-viewport->y)*2;
    gsKit_prim_quad_texture(gsGlobal, spritesheet,
                            x1, y1,         // x1, y1
                            u1, v1,         // u1, v1

                            x1, y1+32,         // x2, y2
                            u1, v2,         // u2, v2

                            x1+32, y1,         // x3, y3
                            u2, v1,         // u3, v3

                            x1+32, y1+32,         // x4, y4
                            u2, v2,         // u4, v4
                            1, TexCol);
}

int block::update()
{
    int arr[6] ={-2, -4, -2, 2, 4, 2};
    if(phase<6)y+=arr[phase];
    else if(phase==6)return 1;
    if(phase < 7)phase++;
    return 0;
}

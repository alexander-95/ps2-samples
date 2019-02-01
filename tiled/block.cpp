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

void block::draw(GSGLOBAL* gsGlobal, int screen_x, int screen_y)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);// set color
    int u1 = 16*sprite, v1 = 0, u2 = 16*(sprite+1), v2 = 16;
    int x1 = (x-screen_x)*2, y1 = (y-screen_y)*2;
    gsKit_prim_quad_texture(gsGlobal, &spritesheet,
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

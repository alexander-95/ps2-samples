//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <stdio.h>
#include "pickup.hpp"

pickup::pickup()
{
    x = 0;
    y = 0;
    width = 8;
    height = 16;
    sprite = 0;
}

pickup::~pickup()
{

}

void pickup::draw(GSGLOBAL* gsGlobal, int screen_x, int screen_y)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    int u1 = width*sprite, v1 = 0, u2 = width*(sprite+1), v2 = height;
    int x1 = x*2 - screen_x*2, y1 = y*2 - screen_y*2;
    gsKit_prim_quad_texture(gsGlobal, &spritesheet,
                            x1,y1,                      // x1, y1
                            u1, v1,                     // u1, v1

                            x1,y1+(2*height),           // x2, y2
                            u1, v2,                     // u2, v2

                            x1+(2*width),y1,            // x3, y3
                            u2, v1,                     // u3, v3

                            x1+(2*width),y1+(2*height), // x4, y4
                            u2, v2,                     // u4, v4
                            1, TexCol);
}

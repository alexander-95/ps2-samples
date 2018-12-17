//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

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

//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include "bird.hpp"

Bird::Bird()
{

}

Bird::~Bird()
{

}

void Bird::draw()
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    // offset used to cycle between bird images for wing flapping animation
    float offset = 0;
    if((cycle & 4) == 0)offset = 17.0f;
    else if((cycle & 8) == 0)offset = 0.0f;
    else offset = 34.0f;

    gsKit_prim_quad_texture(gsGlobal, spritesheet,
                            x, y,                           // x1, y1
                            0.0f+offset, 14.0f+(12*color),  // u1, v1

                            x, y+24,                        // x2, y2
                            0.0f+offset, 26.0f+(12*color),  // u2, v2

                            x+34, y,                        // x3, y3
                            17.0f+offset, 14.0f+(12*color), // u3, v3

                            x+34, y+24,                     // x4, y4
                            17.0f+offset, 26.0f+(12*color), // u4, v4
                            1, TexCol);
    cycle++;
    return;
}

void Bird::reset(int color)
{
    x = 200;
    y = 200;
    vy = 0;
    cycle = 0;
    this->color = color;
}

int Bird::touchingGround()
{
    if(y >= 380)
    {
        return 1;
    }
    return 0;
}

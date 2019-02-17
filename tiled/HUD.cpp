//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <stdio.h>
#include "HUD.hpp"

HUD::HUD()
{
    x = 10;
    y = 5;
}

HUD::~HUD()
{
    
}

void HUD::draw(GSGLOBAL* gsGlobal, int screen_x, int screen_y)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);// set color

    // draw "MARIO" text
    int width = 39, height = 7;
    int u1 = 0, v1 = 0, u2 = width, v2 = height;
    int x1 = x*2, y1 = y*2;
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

    

    // draw "WORLD" text
    width = 39, height = 7;
    u1 = 0, v1 = height, u2 = width, v2 = height*2;
    x1 = (x+100)*2, y1 = y*2;
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

    // draw "TIME" text
    width = 31, height = 7;
    u1 = 0, v1 = height*2, u2 = width, v2 = height*3;
    x1 = (x+200)*2, y1 = y*2;
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

void HUD::drawScore(GSGLOBAL* gsGlobal, int score)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);// set color
    int offset = 0,curr=0, i=0;
        
    do
    {
        curr = score%10;
        gsKit_prim_quad_texture(gsGlobal, &spritesheet,
                                (x+90)-offset, y+22,       // x1, y1
                                0.0f+(8*curr), 35.0f,  // u1, v1

                                (x+90)-offset, y+36,              // x2, y2
                                0.0f+(8*curr), 42.0f, // u2, v2

                                (x+104)-offset, y+22, // x3, y3
                                7.0f+(8*curr), 35.0f,  // u3, v3

                                (x+104)-offset, y+36,        // x4, y4
                                7.0f+(8*curr), 42.0f, // u4, v4
                                3, TexCol);
        offset+=16;
        score/=10;
        i++;
    }
    while(i < 6);
}

void HUD::drawTime(GSGLOBAL* gsGlobal, int time)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);// set color
    int offset = 0,curr=0, i=0;
        
    do
    {
        curr = time%10;
        gsKit_prim_quad_texture(gsGlobal, &spritesheet,
                                (x+490)-offset, y+22,       // x1, y1
                                0.0f+(8*curr), 35.0f,  // u1, v1

                                (x+490)-offset, y+36,              // x2, y2
                                0.0f+(8*curr), 42.0f, // u2, v2

                                (x+504)-offset, y+22, // x3, y3
                                7.0f+(8*curr), 35.0f,  // u3, v3

                                (x+504)-offset, y+36,        // x4, y4
                                7.0f+(8*curr), 42.0f, // u4, v4
                                3, TexCol);
        offset+=16;
        time/=10;
        i++;
    }
    while(time);
}

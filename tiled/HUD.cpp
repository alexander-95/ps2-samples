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
    gsKit_prim_quad_texture(gsGlobal, spritesheet,
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
    gsKit_prim_quad_texture(gsGlobal, spritesheet,
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
    gsKit_prim_quad_texture(gsGlobal, spritesheet,
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
        gsKit_prim_quad_texture(gsGlobal, spritesheet,
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
        gsKit_prim_quad_texture(gsGlobal, spritesheet,
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

void HUD::drawWorld(GSGLOBAL* gsGlobal, int world, int level)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);// set color
    gsKit_prim_quad_texture(gsGlobal, spritesheet,
                            240,27,                // x1, y1
                            0.0f+(8*world), 35.0f, // u1, v1

                            240,41,                // x2, y2
                            0.0f+(8*world), 42.0f, // u2, v2

                            254,27,                // x3, y3
                            7.0f+(8*world), 35.0f, // u3, v3

                            254,41,                // x4, y4
                            7.0f+(8*world), 42.0f, // u4, v4
                            3, TexCol);

    gsKit_prim_quad_texture(gsGlobal, spritesheet,
                            256,27,                // x1, y1
                            8, 56, // u1, v1

                            256,41,                // x2, y2
                            8, 64, // u2, v2

                            270,27,                // x3, y3
                            14, 56, // u3, v3

                            270,41,                // x4, y4
                            14, 64, // u4, v4
                            3, TexCol);

    gsKit_prim_quad_texture(gsGlobal, spritesheet,
                            272,27,                // x1, y1
                            0.0f+(8*level), 35.0f, // u1, v1

                            272,41,                // x2, y2
                            0.0f+(8*level), 42.0f, // u2, v2

                            286,27,                // x3, y3
                            7.0f+(8*level), 35.0f, // u3, v3

                            286,41,                // x4, y4
                            7.0f+(8*level), 42.0f, // u4, v4
                            3, TexCol);
}


void HUD::drawLives(GSGLOBAL* gsGlobal, int lives)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);// set color
    int x1 = 300, y1 = 266;
    gsKit_prim_quad_texture(gsGlobal, spritesheet,
                            x1,y1,                // x1, y1
                            16, 56, // u1, v1

                            x1,y1+14,                // x2, y2
                            16, 63, // u2, v2

                            x1+14,y1,                // x3, y3
                            24, 56, // u3, v3

                            x1+14,y1+14,                // x4, y4
                            24, 63, // u4, v4
                            3, TexCol);

    int offset = 0,curr=0, i=0;
    int x2 = 350, y2 = 266;
    do
    {
        curr = lives%10;
        gsKit_prim_quad_texture(gsGlobal, spritesheet,
                                (x2)-offset, y2,       // x1, y1
                                0.0f+(8*curr), 35.0f,  // u1, v1

                                (x2)-offset, y2+14,    // x2, y2
                                0.0f+(8*curr), 42.0f,  // u2, v2

                                (x2+14)-offset, y2,    // x3, y3
                                7.0f+(8*curr), 35.0f,  // u3, v3

                                (x2+14)-offset, y2+14, // x4, y4
                                7.0f+(8*curr), 42.0f,  // u4, v4
                                3, TexCol);
        offset+=16;
        lives/=10;
        i++;
    }
    while(lives);
}

void HUD::drawString(GSGLOBAL* gsGlobal, int screen_x, int screen_y, char* str)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);// set color
    int offset = 0;
    u8 curr;
    u8 v1 = 42;
    for(int i = 0; str[i]; i++)
    {
        curr = str[i] - 65;
        if(curr < 16) v1 = 42;
        else v1 = 49;
        curr%=16;
        gsKit_prim_quad_texture(gsGlobal, spritesheet,
                                (screen_x)+offset, screen_y,       // x1, y1
                                0.0f+(8*curr), v1,  // u1, v1

                                (screen_x)+offset, screen_y+14,    // x2, y2
                                0.0f+(8*curr), v1+7.0f,  // u2, v2

                                (screen_x+14)+offset, screen_y,    // x3, y3
                                7.0f+(8*curr), v1,  // u3, v3

                                (screen_x+14)+offset, screen_y+14, // x4, y4
                                7.0f+(8*curr), v1+7.0f,  // u4, v4
                                3, TexCol);
        offset += 16;
    }
    
}

void HUD::drawDigit(GSGLOBAL* gsGlobal, int screen_x, int screen_y, u8 digit)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);// set color
    int offset = 0;
    u8 v1 = 35;
    gsKit_prim_quad_texture(gsGlobal, spritesheet,
                            (screen_x)+offset, screen_y,       // x1, y1
                            0.0f+(8*digit), v1,  // u1, v1
                            
                            (screen_x)+offset, screen_y+14,    // x2, y2
                            0.0f+(8*digit), v1+7.0f,  // u2, v2
                            
                            (screen_x+14)+offset, screen_y,    // x3, y3
                            7.0f+(8*digit), v1,  // u3, v3

                            (screen_x+14)+offset, screen_y+14, // x4, y4
                            7.0f+(8*digit), v1+7.0f,  // u4, v4
                            3, TexCol);
}
    

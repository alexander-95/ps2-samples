#include "draw.h"

void drawChar(GSGLOBAL* gsGlobal, GSTEXTURE* font, char ascii, u8 x, u8 y)
{
    ascii -= 32;
    u8 charWidth = 14, charHeight = 18;
    u8 ascii_x = ascii % 18, ascii_y = ascii / 18;
    
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    gsKit_prim_quad_texture(gsGlobal, font,
                            x*charWidth, y*charHeight,     // x1, y1
                            ascii_x*charWidth, ascii_y*charHeight,     // u1, v1

                            x*charWidth, (y+1)*charHeight,   // x2, y2
                            ascii_x*charWidth, (ascii_y+1)*charHeight,   // u2, v2

                            (x+1)*charWidth, y*charHeight,   // x3, y3
                            (ascii_x+1)*charWidth, ascii_y*charHeight,   // u3, v3

                            (x+1)*charWidth, (y+1)*charHeight, // x4, y4
                            (ascii_x+1)* charWidth, (ascii_y+1)*charHeight, // u4, v4
                            6,TexCol);
}

void drawBuffer(GSGLOBAL* gsGlobal, GSTEXTURE* font, char* buffer)
{
    int i, j;
    for(i = 0; i < 28; i++)
    {
        for(j = 0; j < 45; j++) drawChar(gsGlobal, font, buffer[(i*45)+j], j, i);
    }
}

void drawPlatform(GSGLOBAL* gsGlobal, GSTEXTURE* spriteSheet)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    int i = 0;
    while(i < 640)
    {
            gsKit_prim_quad_texture(gsGlobal, spriteSheet,
                                i, 400.0f,   // x1, y1
                                0.0f, 200.0f,   // u1, v1

                                i, 422.0f,   // x2, y2
                                0.0f, 211.0f,   // u2, v2

                                i+24, 400.0f, // x3, y3
                                12, 200.0f, // u3, v3

                                i+24, 422.0f, // x4, y4
                                12, 211.0f, // u4, v4
                                1, TexCol);
            i+=24;
    }
    u64 groundColor = GS_SETREG_RGBAQ(0xDE,0xD8,0x94,0x00,0x00);
    gsKit_prim_quad(gsGlobal,
                    0,422,
                    0,512,
                    640,422,
                    640,512,
                    1, groundColor);
    
}

void drawGameOver(GSGLOBAL* gsGlobal, GSTEXTURE* spriteSheet)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    gsKit_prim_quad_texture(gsGlobal, spriteSheet,
                            320.0f-96.0f, 166.0f-21.0f, // x1, y1
                            52.0f, 87.0f,              // u1, v1

                            320.0f-96.0f, 166.0f+21.0f, // x2, y2
                            52.0f, 108.0f,              // u2, v2

                            320.0f+96.0f, 166.0f-21.0f, // x3, y3
                            148.0f, 87.0f,             // u3, v3

                            320.0f+96.0f, 166.0f+21.0f, // x4, y4
                            148.0f, 108.0f,             // u4, v4
                            3, TexCol); 
}

void drawGetReady(GSGLOBAL* gsGlobal, GSTEXTURE* spriteSheet)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    gsKit_prim_quad_texture(gsGlobal, spriteSheet,
                            320.0f-92.0f, 256.0f-91.0f, // x1, y1
                            52.0f, 109.0f,              // u1, v1

                            320.0f-92.0f, 256.0f+91.0f, // x2, y2
                            52.0f, 200.0f,              // u2, v2

                            320.0f+92.0f, 256.0f-91.0f, // x3, y3
                            144.0f, 109.0f,             // u3, v3

                            320.0f+92.0f, 256.0f+91.0f, // x4, y4
                            144.0f, 200.0f,             // u4, v4
                            2, TexCol); 
}

void drawSaveIcon(GSGLOBAL* gsGlobal, GSTEXTURE* spriteSheet)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    gsKit_prim_quad_texture(gsGlobal, spriteSheet,
                            320.0f-9.0f, 256.0f-12.0f, // x1, y1
                            259.0f, 188.0f,            // u1, v1

                            320.0f-9.0f, 256.0f+12.0f, // x2, y2
                            259.0f, 200.0f,            // u2, v2

                            320.0f+9.0f, 256.0f-12.0f, // x3, y3
                            268.0f, 188.0f,            // u3, v3

                            320.0f+9.0f, 256.0f+12.0f, // x4, y4
                            268.0f, 200.0f,            // u4, v4
                            5, TexCol);
}

void drawNewLabel(GSGLOBAL* gsGlobal, GSTEXTURE* spriteSheet)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    gsKit_prim_quad_texture(gsGlobal, spriteSheet,
                                262.0f-16.0f, 248.0f-7.0f, // x1, y1
                                141.0f, 55.0f,             // u1, v1

                                262.0f-16.0f, 248.0f+7.0f, // x2, y2
                                141.0f, 62.0f,             // u2, v2

                                262.0f+16.0f, 248.0f-7.0f, // x3, y3
                                157.0f, 55.0f,             // u3, v3

                                262.0f+16.0f, 248.0f+7.0f, // x4, y4
                                157.0f, 62.0f,             // u4, v4
                                4, TexCol);
}

void drawMedal(GSGLOBAL* gsGlobal, GSTEXTURE* spriteSheet, int score, int highScore)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    int medal = 0, new_medal = 0;
    if(score >=10 && score < 20)
    {
        medal=0;
        if(highScore < 10) new_medal = 1;
    }
    else if(score >= 20 && score <= 30)
    {
        medal=1;
        if(highScore < 20) new_medal = 1;
    }
    else if(score > 30 && score < 40)
    {
        medal = 2;
        if(highScore <= 30) new_medal = 1;
    }
    else
    {
        medal = 3;
        new_medal = 1;
    }

    if(score >= 10)
    {
        gsKit_prim_quad_texture(gsGlobal, spriteSheet,
                                256.0f-22.0f, 264.0f-22.0f, // x1, y1
                                53.0f+(22*medal), 55.0f,    // u1, v1

                                256.0f-22.0f, 264.0f+22.0f, // x2, y2
                                53.0f+(22*medal), 78.0f,    // u2, v2

                                256.0f+22.0f, 264.0f-22.0f, // x3, y3
                                75.0f+(22*medal), 55.0f,    // u3, v3

                                256.0f+22.0f, 264.0f+22.0f, // x4, y4
                                75.0f+(22*medal), 78.0f,    // u4, v4
                                3, TexCol);
    }
    if(new_medal)drawNewLabel(gsGlobal, spriteSheet);
}

void drawEnd(GSGLOBAL* gsGlobal, GSTEXTURE* spriteSheet, int score, int highScore)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    gsKit_prim_quad_texture(gsGlobal, spriteSheet,
                            320.0f-112.0f, 256.0f-56.0f, // x1, y1
                            146.0f, 143.0f,              // u1, v1

                            320.0f-112.0f, 256.0f+56.0f, // x2, y2
                            146.0f, 200.0f,              // u2, v2

                            320.0f+112.0f, 256.0f-56.0f, // x3, y3
                            259.0f, 143.0f,              // u3, v3

                            320.0f+112.0f, 256.0f+56.0f, // x4, y4
                            259.0f, 200.0f,              // u4, v4
                            2, TexCol);

    int curr = 0, offset = 0;

    drawMedal(gsGlobal, spriteSheet, score, highScore);
    if(score > highScore)highScore = score;

    // draw score
    do
    {
        curr = score%10;
        gsKit_prim_quad_texture(gsGlobal, spriteSheet,
                                400.0f-offset, 236.0f,  // x1, y1
                                52.0f+(6*curr), 41.0f,  // u1, v1

                                400.0f-offset, 250.0f, // x2, y2
                                52.0f+(6*curr), 48.0f, // u2, v2

                                412.0f-offset, 236.0f, // x3, y3
                                58.0f+(6*curr), 41.0f, // u3, v3

                                412.0f-offset, 250.0f, // x4, y4
                                58.0f+(6*curr), 48.0f, // u4, v4
                                3, TexCol);
        offset+=14;
        score/=10;
    }
    while(score);
    offset = 0;

    do
    {
        curr = highScore%10;
        gsKit_prim_quad_texture(gsGlobal, spriteSheet,
                                400.0f-offset, 276.0f, // x1, y1
                                52.0f+(6*curr), 41.0f, // u1, v1

                                400.0f-offset, 290.0f, // x2, y2
                                52.0f+(6*curr), 48.0f, // u2, v2

                                412.0f-offset, 276.0f, // x3, y3
                                58.0f+(6*curr), 41.0f, // u3, v3

                                412.0f-offset, 290.0f, // x4, y4
                                58.0f+(6*curr), 48.0f, // u4, v4
                                3, TexCol);
        offset+=14;
        highScore/=10;
    }
    while(highScore);
    drawGameOver(gsGlobal, spriteSheet);
}

void drawBackground(GSGLOBAL* gsGlobal, GSTEXTURE* sprites)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);

    u64 skycolor = GS_SETREG_RGBAQ(0x4E,0xC0,0xCA,0x00,0x00);
    gsKit_prim_quad(gsGlobal,
                    0,0,
                    0,512,
                    640,0,
                    640,512,
                    0, skycolor);

    u64 bushcolor = GS_SETREG_RGBAQ(0x5E,0xE2,0x70,0x00,0x00);
    gsKit_prim_quad(gsGlobal,
                    0,386,
                    0,512,
                    640,386,
                    640,512,
                    0, bushcolor);
    
    gsKit_prim_quad_texture(gsGlobal, sprites,
                                0.0f, 308.0f,     // x1, y1
                                0.0f, 217.0f,     // u1, v1

                                0.0f, 386.0f,   // x2, y2
                                0.0f, 256.0f,   // u2, v2

                                640.0f, 308.0f,   // x3, y3
                                320.0f, 217.0f,   // u3, v3

                                640.0f, 386.0f, // x4, y4
                                320.0f, 256.0f, // u4, v4
                                0,TexCol);
    
}

void drawFont(GSGLOBAL* gsGlobal, GSTEXTURE* font)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    gsKit_prim_quad_texture(gsGlobal, font,
                            0.0f, 0.0f,     // x1, y1
                            0.0f, 0.0f,     // u1, v1

                            0.0f, 128.0f,   // x2, y2
                            0.0f, 128.0f,   // u2, v2

                            256.0f, 0.0f,   // x3, y3
                            256.0f, 0.0f,   // u3, v3

                            256.0f, 128.0f, // x4, y4
                            256.0f, 128.0f, // u4, v4
                            6,TexCol);
}

void drawScore(GSGLOBAL* gsGlobal, int score, GSTEXTURE* sprites)
{
    int length, curr;
    int temp = score;
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);

    for(length=0; temp>0; temp/=10)length++;
    if(score==0)length=1;

    int height = 36, width = 24, space = 0;
    int totalWidth = (width * length) + (space * (length-1));
    totalWidth /= 2;
    totalWidth -= width;
    int p = 320 + totalWidth;
    while(score)
    {
        curr = score % 10;
        gsKit_prim_quad_texture(gsGlobal, sprites,
                                p, 100.0f-height,       // x1, y1
                                52.0f+(12*curr), 0.0f,  // u1, v1

                                p, 100.0f,              // x2, y2
                                52.0f+(12*curr), 18.0f, // u2, v2

                                p+width, 100.0f-height, // x3, y3
                                64.0f+(12*curr), 0.0f,  // u3, v3

                                p+width, 100.0f,        // x4, y4
                                64.0f+(12*curr), 18.0f, // u4, v4
                                3, TexCol);
        p -= width + space;
        score/=10;
    }
}

void drawTitleScreen(GSGLOBAL* gsGlobal, GSTEXTURE* spritesheet)
{
    u64 TexCol = GS_SETREG_RGBAQ(0xFF,0xFF,0xFF,0x00,0x00);
    gsKit_mode_switch(gsGlobal, GS_PERSISTENT);
    gsKit_prim_quad_texture(gsGlobal, spritesheet,
                            320.0f-66.0f, 256.0f-16.0f,  // x1, y1
                            172.0f, 0.0f,              // u1, v1

                            320.0f-66.0f, 256.0f+16.0f, // x2, y2
                            172.0f, 17.0f,              // u2, v2

                            320.0f+66.0f, 256.0f-16.0f, // x3, y3
                            239.0f, 0.0f,             // u3, v3

                            320.0f+66.0f, 256.0f+16.0f, // x4, y4
                            239.0f, 17.0f,             // u4, v4
                            3, TexCol);
    gsKit_queue_exec(gsGlobal);
    gsKit_sync_flip(gsGlobal);
    gsKit_mode_switch(gsGlobal, GS_ONESHOT);
}

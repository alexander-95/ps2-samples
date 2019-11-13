//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#ifndef HUD_HPP
#define HUD_HPP

#include <gsKit.h>

class HUD
{
public:
    HUD();
    ~HUD();
    void draw(int screen_x, int screen_y);
    void drawScore(int score);
    void drawTime(int score);
    void drawWorld(int world, int level);
    void drawLives(int lives);
    void drawString(int screen_x, int screen_y, char* str);
    void drawDigit(int screen_x, int screen_y, u8 digit);
    int x;
    int y;
    int width;
    int height;
    GSTEXTURE* spritesheet;
    static GSGLOBAL* gsGlobal;
};

#endif

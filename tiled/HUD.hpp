//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>

class HUD
{
public:
    HUD();
    ~HUD();
    void draw(GSGLOBAL* gsGlobal, int screen_x, int screen_y);
    void drawScore(GSGLOBAL* gsGlobal, int score);
    void drawTime(GSGLOBAL* gsGlobal, int score);
    void drawWorld(GSGLOBAL* gsGlobal, int world, int level);
    void drawLives(GSGLOBAL* gsGlobal, int lives);
    int x;
    int y;
    int width;
    int height;
    GSTEXTURE spritesheet;
};

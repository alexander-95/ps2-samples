//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>
#include "map.hpp"

class character
{
public:
    character();
    ~character();
    void draw(GSGLOBAL* gsGlobal, int screen_x, int screen_y);
    int canMoveDown(map* level, u8* solid, int n);
    int canMoveUp(map* level, u8* solid, int n);
    int canMoveLeft(map* level, u8* solid, int n);
    int canMoveRight(map* level, u8* solid, int n);
    int traverse(map* level, u8* solid);
    int gravity(map* level, u8* solid, u8 tick, int gravity);
    int isOnScreen(int screen_x);
    int isTouching(character* c);
    u8 standingOnPipe(map* level);
    int x;
    int y;
    int vy;
    int sprite; // sprite number to draw
    u8 hflip; // horizontal flip for sprite
    u8 direction;
    u8 gate;
    u8 width;
    u8 height;
    GSTEXTURE spritesheet;
};

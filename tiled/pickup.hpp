//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>

#ifndef PICKUP_HPP
#define PICKUP_HPP

class pickup
{
public:
    pickup();
    ~pickup();
    void draw(GSGLOBAL* gsGlobal, int screen_x, int screen_y);
    void update();
    int x;
    int y;
    int width;
    int height;
    int sprite;
    u8 activated;
    u8 phase;
    u8 type;
    GSTEXTURE spritesheet;
};

#endif

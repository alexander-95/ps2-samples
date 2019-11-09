//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>
#include "utils.h"

#ifndef PICKUP_HPP
#define PICKUP_HPP

class pickup
{
public:
    pickup();
    ~pickup();
    void draw();
    void update();
    void print();
    int x;
    int y;
    int width;
    int height;
    int sprite;
    u8 activated;
    u8 phase;
    u8 type;
    GSTEXTURE* spritesheet;
    static GSGLOBAL* gsGlobal;
    static point* viewport;
};

#endif

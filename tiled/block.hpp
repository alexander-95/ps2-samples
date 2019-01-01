//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>
#include "map.hpp"

class block
{
public:
    block();
    ~block();
    void draw(GSGLOBAL* gsGlobal, int screen_x, int screen_y);
    int update();
    int x;
    int y;
    int vy;
    int sprite;
    int phase;
    GSTEXTURE spritesheet;
};

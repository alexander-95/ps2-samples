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
    void draw(int screen_x, int screen_y);
    int update();
    int x;
    int y;
    int vy;
    int sprite;
    int phase;
    u8 active;
    GSTEXTURE* spritesheet;
    static GSGLOBAL* gsGlobal;
};

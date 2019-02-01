//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>

class pickup
{
public:
    pickup();
    ~pickup();
    void draw(GSGLOBAL* gsGlobal, int screen_x, int screen_y);
    int x;
    int y;
    int width;
    int height;
    int sprite;
    GSTEXTURE spritesheet;
};

//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>

class character
{
public:
    character();
    ~character();
    void draw(GSGLOBAL* gsGlobal);
    int x;
    int y;
    int sprite;
    GSTEXTURE spritesheet;
};

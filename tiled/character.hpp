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
    void draw(GSGLOBAL* gsGlobal);
    int canMoveDown(map* level, u8* map_data, u8* solid, int scale_factor, int screen_x, int screen_y, int n);
    int canMoveUp(map* level, u8* map_data, u8* solid, int scale_factor, int screen_x, int screen_y, int n);
    int canMoveLeft(map* level, u8* map_data, u8* solid, int scale_factor, int screen_x, int screen_y, int n);
    int canMoveRight(map* level, u8* map_data, u8* solid, int scale_factor, int screen_x, int screen_y, int n);
    int x;
    int y;
    int vy;
    int sprite;
    GSTEXTURE spritesheet;
};

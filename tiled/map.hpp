//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>

#ifndef MAP_HPP
#define MAP_HPP

class map
{
public:
    map();
    ~map();
    int get_box(int x, int y);
    int get_index(int x, int y);
    int width; //map width in tiles
    int height;
    int absoluteHeight;
    int tile_width;
    int tile_height;
    GSTEXTURE spritesheet;
    u8* data;
};

#endif

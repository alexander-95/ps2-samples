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
    int width; //map width in tiles
    int height;
    int tile_width;
    int tile_height;
    GSTEXTURE spritesheet;
};

#endif

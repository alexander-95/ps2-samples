//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include "map.hpp"

map::map()
{
    width = 224;
    height = 15;
    tile_width = 16;
    tile_height = 16;
}

map::~map()
{
    
}

int map::get_box(int x, int y)
{
    int index = get_index(x, y);
    return data[index];
}


int map::get_index(int x, int y)
{
    int index_x = x / tile_width;
    int index_y = y / tile_height;
    return (index_y*width)+index_x;
}

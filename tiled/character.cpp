//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <stdio.h>
#include "character.hpp"

character::character()
{
    x = 0;
    y = 0;
    sprite = 0;
    hflip = 0;
    width = 16;
    height = 16;
    animationMode = 0;
    animationFrame = 0;
    collisionDetection = 1;
    printf("character spawned\n");
}

character::~character()
{
    
}

void character::draw(GSGLOBAL* gsGlobal, int screen_x, int screen_y)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);// set color
    int u1 = width*sprite, v1 = 0, u2 = width*(sprite+1), v2 = height;
    int x1 = x*2 - screen_x*2, y1 = y*2 - screen_y*2;
    if(hflip)
    {
        u1^=u2;
        u2^=u1;
        u1^=u2;
    }
    gsKit_prim_quad_texture(gsGlobal, &spritesheet,
                            x1,y1,         // x1, y1
                            u1, v1,         // u1, v1

                            x1,y1+(2*height),         // x2, y2
                            u1, v2,         // u2, v2

                            x1+(2*width),y1,         // x3, y3
                            u2, v1,         // u3, v3

                            x1+(2*width),y1+(2*height),         // x4, y4
                            u2, v2,         // u4, v4
                            1, TexCol);
}

int character::canMoveDown(map* level, u8* solid, int n )
{
    if(!collisionDetection)return 1;
    int x1 = x;
    int y1 = y+n+(height-1);
    int x2 = x+(width-1);
    int y2 = y+n+(height-1);
    
    // figure out which tile mario is running into
    int tile_x1 = (x1 / level->tile_width);
    int tile_y1 = (y1 / level->tile_height);
    int index1 = tile_y1 * level->width + tile_x1;
    int value1 = level->data[index1];

    int tile_x2 = (x2 / level->tile_width);
    int tile_y2 = (y2 / level->tile_height);
    int index2 = tile_y2 * level->width + tile_x2;
    int value2 = level->data[index2];

    if(!solid[value1] && !solid[value2]) return 1;
    else return 0;
}

int character::canMoveUp(map* level, u8* solid, int n )
{
    if(!collisionDetection)return 1;
    int x1 = x;
    int y1 = y-n;
    int x2 = x+(width-1);
    int y2 = y-n;
    
    // figure out which tile mario is running into
    int tile_x1 = (x1 / level->tile_width);
    int tile_y1 = (y1 / level->tile_height);
    int index1 = tile_y1 * level->width + tile_x1;
    int value1 = level->data[index1];

    int tile_x2 = (x2 / level->tile_width);
    int tile_y2 = (y2 / level->tile_height);
    int index2 = tile_y2 * level->width + tile_x2;
    int value2 = level->data[index2];

    if(!solid[value1] && !solid[value2]) return 1;
    else return 0;
}

int character::canMoveRight(map* level, u8* solid, int n )
{
    if(!collisionDetection)return 1;
    int x1 = x+n+(width-1);
    int y1 = y;
    int x2 = x+n+(width-1);
    int y2 = y+(height-1);
    
    // figure out which tile mario is running into
    int tile_x1 = (x1 / level->tile_width);
    int tile_y1 = (y1 / level->tile_height);
    int index1 = tile_y1 * level->width + tile_x1;
    int value1 = level->data[index1];

    //printf("tile %d, %d\n", tile_x1, tile_y1);
    
    int tile_x2 = (x2 / level->tile_width);
    int tile_y2 = (y2 / level->tile_height);
    int index2 = tile_y2 * level->width + tile_x2;
    int value2 = level->data[index2];

    if(!solid[value1] && !solid[value2]) return 1;
    else return 0;
}

int character::canMoveLeft(map* level, u8* solid, int n )
{
    if(!collisionDetection)return 1;
    int x1 = x-n;
    int y1 = y;
    int x2 = x-n;
    int y2 = y+(height-1);
    
    // figure out which tile mario is running into
    int tile_x1 = (x1 / level->tile_width);
    int tile_y1 = (y1 / level->tile_height);
    int index1 = tile_y1 * level->width + tile_x1;
    int value1 = level->data[index1];

    int tile_x2 = (x2 / level->tile_width);
    int tile_y2 = (y2 / level->tile_height);
    int index2 = tile_y2 * level->width + tile_x2;
    int value2 = level->data[index2];

    if(!solid[value1] && !solid[value2] && x-n >= 0) return 1;
    else return 0;
}

void character::traverse(map* level, u8* solid)
{
    if(direction)
    {
        if(canMoveRight(level,solid, 1))x++;
        else direction = 0;
    }
    else
    {
        if(canMoveLeft(level, solid,1))x--;
        else direction = 1;
    }
}

void character::gravity(map* level, u8* solid, u8 tick, int gravity )
{
    if(vy > 0)
    {
        if(canMoveDown(level, solid, vy))
        {
            y += vy;
            if((tick & 3) == 0)vy += gravity;
        }
        else
        {
            while(vy > 0 && !canMoveDown(level, solid, vy))vy--;
            y += vy;
            vy = 0;
        }
    }
    else if(vy < 0 || canMoveDown(level, solid, 1)) // jumping
    {
        if(canMoveUp(level, solid, vy*(-1)))
        {
            y += vy;
            if((tick & 3) == 0)vy += gravity;
        }
        else
        {
            while(vy < 0 && !canMoveUp(level, solid, vy*(-1)))vy++;
            y += vy;
        }
    }
}

int character::isOnScreen(int screen_x)
{
    if(x > screen_x && x < screen_x + 320)return 1;
    else return 0;
}

int character::isTouching(character* c)
{
    if(!collisionDetection)return 0;
    u8 collision_x = 0, collision_y = 0;
    if(c->x > x - c->width && c->x < x + width) collision_x = 1;
    if(c->y > y - c->height && c->y < y + height) collision_y = 1;
    if(collision_x && collision_y) return 1;
    else return 0;
}

int character::pickedup(pickup* p)
{
    if(!p->activated)return 0;
    u8 collision_x = 0, collision_y = 0;
    if(p->x > x - p->width && p->x < x + width) collision_x = 1;
    if(p->y > y - p->height && p->y < y + height) collision_y = 1;
    if(collision_x && collision_y) return 1;
    else return 0;
}

u8 character::standingOnPipe(map* level)
{
    int x1 = x, y1 = y+height+1;
    int x2 = x+width, y2 = y+height+1;
    
    // figure out which tile mario is running into
    int tile_x1 = (x1 / level->tile_width);
    int tile_y1 = (y1 / level->tile_height);
    int index1 = tile_y1 * level->width + tile_x1;
    int value1 = level->data[index1];

    int tile_x2 = (x2 / level->tile_width);
    int tile_y2 = (y2 / level->tile_height);
    int index2 = tile_y2 * level->width + tile_x2;
    int value2 = level->data[index2];
    if(value1 == 16 && value2 == 17) return 1;
    else return 0;

}

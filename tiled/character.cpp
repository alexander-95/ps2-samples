//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <stdio.h>
#include "character.hpp"

character::character()
{
    x = 0;
    y = 0;
    vy = 0;
    sprite = 0;
    hflip = 0;
    width = 16;
    height = 16;
    animationMode = 0;
    animationFrame = 0;
    collisionDetection = 1;
    activated = 1;
    printf("character spawned\n");
}

character::~character()
{
    
}

void character::draw()
{
    if(!activated)return;
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);// set color
    int u1 = width*sprite, v1 = 0, u2 = width*(sprite+1), v2 = height;
    int x1 = x*2 - viewport->x*2, y1 = y*2 - viewport->y*2;
    if(hflip)
    {
        u1^=u2;
        u2^=u1;
        u1^=u2;
    }
    gsKit_prim_quad_texture(gsGlobal, spritesheet,
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
    if(!activated)return;
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
    if(!activated)return;
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

int character::isOnScreen()
{
    if(x > viewport->x && x < viewport->x + 320)return 1;
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
u8 character::pipeOnRight(map* level)
{
    int x1 = x+width+1, y1 = y;
    int x2 = x+width+1, y2 = y+height-1;
    
    // figure out which tile mario is running into
    int tile_x1 = (x1 / level->tile_width);
    int tile_y1 = (y1 / level->tile_height);
    int index1 = tile_y1 * level->width + tile_x1;
    int value1 = level->data[index1];

    int tile_x2 = (x2 / level->tile_width);
    int tile_y2 = (y2 / level->tile_height);
    int index2 = tile_y2 * level->width + tile_x2;
    int value2 = level->data[index2];

    //printf("value1 = %d\nvalue2 = %d\n", value1, value2);
    
    if((value1 == 40 || value1 == 48) && value2 == 48) return 1;
    else return 0;

}

void character::print()
{
    printf("position: <%d, %d> vy: %d\n", x, y, vy);
}

void character::reactToControllerInput(controller* pad, u8 tick, map* level, u8* solid, int scale_factor, u8* superMario, u8* frameByFrame)
{
    if(animationMode) return;
    if(pad->left())
    {
        hflip = 1;
        if(tick&1) gate++;
        gate &= 3;
        if(!canMoveDown(level, solid, 2))
        {
            if(gate & 1) sprite = 2;
            else
            {
                if(gate)sprite = 1;
                else sprite = 3;
            }
        }
        if(canMoveLeft(level, solid, 2))
        {
            x-=2;
            if(viewport->x > 0 && viewport->y == 0)viewport->x-=2;
        }
    }
    if(pad->right())
    {
        hflip = 0;
        if(tick&1)gate++;
        gate &= 3;
        if(!canMoveDown(level, solid, 2))
        {
            if(gate & 1)sprite = 2;
            else
            {
                if(!gate) sprite = 1;
                else sprite = 3;
            }
        }
        if(canMoveRight(level, solid, 2))
        {
            x+=2;
            if(viewport->x+(gsGlobal->Width/(2*scale_factor)) < (level->width*level->tile_width) &&
               x > (gsGlobal->Width/(2*scale_factor)) &&
               viewport->y == 0) viewport->x+=2;
        }
    }
    if(pad->up() || pad->x(1))
    {
        sprite = 5;
        if(y > 0)
        {
            if(canMoveUp(level, solid, 2) &&
               !canMoveDown(level, solid, 2))vy = -6;//mario.y-=2;
        }
    }
    if(pad->down())
    {
        if(standingOnPipe(level))
        {
            printf("standing on pipe\n");
            animationMode = 1;
        }
    }
    if(pad->triangle(1))
    {
        if((*superMario))
        {
            *superMario = 0;
            height = 16;
            width = 16;
            y+=16;
        }
        else
        {
            *superMario = 1;
            height = 32;
            width = 18;
            y -= 16;
        }
    }
    if(pad->square(1))
    {
        collisionDetection ^= 1;
    }
    
    if(pad->circle(1))
    {
        if(!(*frameByFrame))*frameByFrame = 1;
    }
    if(*frameByFrame)
    {
        while(!pad->circle(1))pad->read();
    }
}

void character::doAnimation(u8 tick, u8* superMario, u8* restart)
{
    // mario is entering a pipe
    if(animationMode == 1)
    {
        if(animationFrame < 10)
        {
            if((tick & 7) == 0)
            {
                y+=2;
                animationFrame++;
            }
        }
        else
        {
            animationMode = 0;
            animationFrame = 0;
            viewport->x = 2368;
            viewport->y = 240;
            x = 2384;
            y = 240;
        }
    }
    // growing into big mario
    else if(animationMode == 2)
    {
        u8 arr1[10] = {0, 13, 0, 13, 0, 13, 15, 0, 13, 15};
        char arr2[10] = {0, -16, 16,-16,16,-16,0,16,-16,0};
        
        if(animationFrame < 10)
        {
            if((tick & 3) == 0)
            {
                sprite = arr1[animationFrame];
                y += arr2[animationFrame];
                if(sprite == 0)
                {
                    height = 16;
                    width = 16;
                }
                else
                {
                    height = 32;
                    width = 18;                    
                }
                animationFrame++;
            }
        }
        else
        {
            animationMode = 0;
            animationFrame = 0;
            sprite = 0;
            *superMario = 1;
        }
    }
    // shrinking back down to small mario
    else if(animationMode == 3)
    {
        
    }
    // death animation
    else if(animationMode == 4)
    {
        //u8 arr[10] = {10,10,10,10,10,-20,-20,-20,-20,-20};
        sprite = 13;
        collisionDetection = 0;
        if(animationFrame < 15)
        {
            if((tick & 1) == 0)
            {
                y -= 5;
                animationFrame++;
            }
        }
        else if(animationFrame < 80)
        {
            if((tick & 1) == 0)
            {
                y += 5;
                animationFrame++;
            }
        }
        else
        {
            *restart = 1;
        }
    }
    else if(animationMode == 5)
    {
        vy = 0;
        if(animationFrame < 10)
        {
            if((tick & 7) == 0)
            {
                x+=2;
                animationFrame++;
            }
        }
        else
        {
            animationMode = 0;
            animationFrame = 0;
            viewport->x = 0;
            viewport->y = 0;
            x = 0;
            y = 192;
        }
    }
}

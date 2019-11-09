//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>
#include "map.hpp"
#include "pickup.hpp"
#include "controller.hpp"
#include "utils.h"

class character
{
public:
    character();
    ~character();
    void draw();
    int canMoveDown(map* level, u8* solid, int n);
    int canMoveUp(map* level, u8* solid, int n);
    int canMoveLeft(map* level, u8* solid, int n);
    int canMoveRight(map* level, u8* solid, int n);
    void traverse(map* level, u8* solid);
    void gravity(map* level, u8* solid, u8 tick, int gravity);
    int isOnScreen();
    int isTouching(character* c);
    int pickedup(pickup* p);
    u8 standingOnPipe(map* level);
    u8 pipeOnRight(map* level);
    void reactToControllerInput(controller* pad, u8 tick, map* level, u8* solid, int scale_factor, u8* superMario, u8* frameByFrame);
    void doAnimation(u8 tick, u8* superMario, u8* restart);
    void print();
    
    int x;
    int y;
    int vy;
    int sprite; // sprite number to draw
    u8 hflip; // horizontal flip for sprite
    u8 direction;
    u8 gate;
    u8 width;
    u8 height;
    u8 animationMode;
    u8 animationFrame;
    u8 collisionDetection;
    u8 activated;
    GSTEXTURE* spritesheet;
    
    static GSGLOBAL* gsGlobal;
    static point* viewport;
};

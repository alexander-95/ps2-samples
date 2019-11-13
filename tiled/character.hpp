//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <gsKit.h>
#include "levelBuilder.hpp"
#include "pickup.hpp"
#include "controller.hpp"
#include "utils.h"

class character
{
public:
    character();
    ~character();
    void draw();
    int canMoveDown(LevelBuilderBase* level, int n);
    int canMoveUp(LevelBuilderBase* level, int n);
    int canMoveLeft(LevelBuilderBase* level, int n);
    int canMoveRight(LevelBuilderBase* level, int n);
    void traverse(LevelBuilderBase* level);
    void gravity(LevelBuilderBase* level, u8 tick, int gravity);
    int isOnScreen();
    int isTouching(character* c);
    //void doAnimation(u8 tick, u8* superMario, u8* restart);
    void print();
    
    point worldCoordinates;
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

class PlayableCharacter : public character
{
public:
    int pickedup(pickup* p);
    u8 standingOnPipe(LevelBuilderBase* level);
    u8 pipeOnRight(LevelBuilderBase* level);
    void reactToControllerInput(controller* pad, u8 tick, LevelBuilderBase* level, int scale_factor, u8* superMario, u8* frameByFrame);
    void doAnimation(u8 tick, u8* superMario, u8* restart);
};

#endif

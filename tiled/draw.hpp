//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#ifndef DRAW_HPP
#define DRAW_HPP

#include <gsKit.h>
#include "controller.hpp"
#include "levelBuilder.hpp"
#include "HUD.hpp"
#include "utils.h"
#include "character.hpp"

void drawTile(GSGLOBAL* gsGlobal, GSTEXTURE* spritesheet, int scale_factor, Level* level, point start, int value, int z);
void drawScreen(GSGLOBAL* gsGlobal, GSTEXTURE* spritesheet, int scale_factor, Level* level, point viewport);
void drawLevelStart(GSGLOBAL* gsGlobal, HUD* hud, PlayableCharacter* mario, int score, int lives);
void drawStartScreen(GSGLOBAL* gsGlobal, controller* pad, HUD* hud, Level* level1);
#endif

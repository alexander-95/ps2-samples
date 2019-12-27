#ifndef DRAW_HPP_
#define DRAW_HPP_

#include <gsKit.h>
#include "log.hpp"

void drawPlatform(GSGLOBAL* gsGlobal, GSTEXTURE* spriteSheet);
void drawGameOver(GSGLOBAL* gsGlobal, GSTEXTURE* spriteSheet);
void drawGetReady(GSGLOBAL* gsGlobal, GSTEXTURE* spriteSheet);
void drawSaveIcon(GSGLOBAL* gsGlobal, GSTEXTURE* spriteSheet);
void drawNewLabel(GSGLOBAL* gsGlobal, GSTEXTURE* spriteSheet);
void drawMedal(GSGLOBAL* gsGlobal, GSTEXTURE* spriteSheet, int score, int highScore);
void drawEnd(GSGLOBAL* gsGlobal, GSTEXTURE* spriteSheet, int score, int highScore);
void drawBackground(GSGLOBAL* gsGlobal, GSTEXTURE* sprites, int nightMode);
void drawFont(GSGLOBAL* gsGlobal, GSTEXTURE* font);
void drawScore(GSGLOBAL* gsGlobal, int score, GSTEXTURE* sprites);
void drawTitleScreen(GSGLOBAL* gsGlobal, GSTEXTURE* spritesheet);

#endif

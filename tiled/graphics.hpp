//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#ifndef GRAPHICS_HPP_
#define GRAPHICS_HPP_

#include <gsKit.h>

GSTEXTURE loadTexture(GSGLOBAL* gsGlobal, u32* tex_array, int width, int height, int psm);
void gsKit_texture_abgr(GSGLOBAL* gsGlobal, GSTEXTURE* tex, u32* arr, u32 width, u32 height, int psm);
void configureGraphics(GSGLOBAL* gsGlobal);

#endif

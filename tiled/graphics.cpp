//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include "graphics.hpp"

void gsKit_texture_abgr(GSGLOBAL* gsGlobal, GSTEXTURE* tex, u32* arr, u32 width, u32 height, int psm)
{
    u32 VramTextureSize = gsKit_texture_size(width, height, psm);

    tex->Width = width;
    tex->Height = height;
    tex->PSM = psm;
    tex->ClutPSM = 0;
    tex->TBW = 1;
    tex->Mem = arr;
    tex->Clut = NULL;
    tex->Vram = gsKit_vram_alloc(gsGlobal, VramTextureSize, GSKIT_ALLOC_USERBUFFER);
    tex->VramClut = 0;
    tex->Filter = GS_FILTER_NEAREST;
    tex->Delayed = 0;

    gsKit_texture_upload(gsGlobal, tex);
}

GSTEXTURE loadTexture(GSGLOBAL* gsGlobal, u32* tex_array, int width, int height, int psm)
{
    GSTEXTURE tex;
    tex.Width=width;
    tex.Height=height;
    tex.PSM = psm;
    gsKit_texture_abgr(gsGlobal, &tex, tex_array, width, height, psm );
    return tex;
}

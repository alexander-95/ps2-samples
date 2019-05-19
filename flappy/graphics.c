#include "graphics.h"
#include "draw.h"

void gsKit_texture_abgr(GSGLOBAL* gsGlobal, GSTEXTURE* tex, u32* arr, u32 width, u32 height)
{
    u32 VramTextureSize = gsKit_texture_size(width, height, GS_PSM_CT32);

    tex->Width = width;
    tex->Height = height;
    tex->PSM = GS_PSM_CT32;
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
    gsKit_texture_abgr(gsGlobal, &tex, tex_array, width, height );
    return tex;
}

void updateFrame(GSGLOBAL* gsGlobal, GSTEXTURE* font, char* buffer)
{
    drawBuffer(gsGlobal, font, buffer);
    
    gsKit_sync_flip(gsGlobal);
    gsKit_queue_exec(gsGlobal);
    gsKit_queue_reset(gsGlobal->Per_Queue);
    gsKit_clear(gsGlobal, 0);
}

void configureGraphics(GSGLOBAL* gsGlobal)
{
    gsGlobal->Mode = GS_MODE_PAL;
    gsGlobal->Width=640;
    gsGlobal->Height=512;
    //gsGlobal->PSM = GS_PSM_CT32;
    gsGlobal->PSMZ = GS_PSMZ_16S;
    gsGlobal->ZBuffering = GS_SETTING_ON;
    gsGlobal->PrimAlphaEnable = GS_SETTING_ON;
    gsGlobal->DoubleBuffering = GS_SETTING_ON;
}


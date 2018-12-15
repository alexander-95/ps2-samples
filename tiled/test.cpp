//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>

#include "spritesheet.h"

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

int main()
{
    GSGLOBAL* gsGlobal = gsKit_init_global();
    //gsGlobal->Mode = GS_MODE_PAL;
    gsGlobal->Width=640;
    gsGlobal->Height=512;
    gsGlobal->PSMZ = GS_PSMZ_32;
    gsGlobal->PSM = GS_PSM_CT32;
    gsGlobal->ZBuffering = GS_SETTING_ON;
    gsGlobal->PrimAlphaEnable = GS_SETTING_ON;
    gsGlobal->DoubleBuffering = GS_SETTING_ON;

    GSTEXTURE spriteSheet;
    spriteSheet.Width = 128;
    spriteSheet.Height = 64;
    spriteSheet.PSM = GS_PSM_CT32;

    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

    //Initialize the DMAC
    dmaKit_chan_init(DMA_CHANNEL_GIF);
    gsKit_init_screen(gsGlobal);
    gsKit_set_clamp(gsGlobal, GS_CMODE_CLAMP);
    gsKit_mode_switch(gsGlobal, GS_ONESHOT);

    gsKit_texture_abgr(gsGlobal, &spriteSheet, spritesheet_array, spriteSheet.Width, spriteSheet.Height );
    u64 White = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);// set color
    
    while(1)
    {
        gsKit_prim_quad_texture(gsGlobal, &spriteSheet,
                                0,0,                                // x1, y1
                                0,0,         // u1, v1
                                    
                                0,256,                  // x2, y2
                                0,64,     // u2, y2

                                512,0,                   // x3, y3
                                128,0,     // u3, v3
                            
                                512,256,     // x4, y4
                                128,64, // u4, v4
                                0,White);
        
        gsKit_sync_flip(gsGlobal);
        gsKit_queue_exec(gsGlobal);
        gsKit_queue_reset(gsGlobal->Per_Queue);
        gsKit_clear(gsGlobal, 0);
    }
}

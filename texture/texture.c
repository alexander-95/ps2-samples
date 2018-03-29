//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <stdio.h>
#include <malloc.h>
#include <gsKit.h>
#include <dmaKit.h>
#include <gsToolkit.h>

int main(int argc, char *argv[])
{
    GSGLOBAL *gsGlobal = gsKit_init_global();
    GSTEXTURE tex;
    u64 White = GS_SETREG_RGBAQ(0xFF,0xFF,0xFF,0x00,0x00);
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);

    gsGlobal->PSM = GS_PSM_CT24;
    gsGlobal->PSMZ = GS_PSMZ_16S;
    
    dmaKit_init(D_CTRL_RELE_OFF,D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

    // Initialize the DMAC
    dmaKit_chan_init(DMA_CHANNEL_GIF);
    gsKit_init_screen(gsGlobal);
    gsKit_mode_switch(gsGlobal, GS_PERSISTENT);
    gsKit_clear(gsGlobal, White);

    gsKit_texture_bmp(gsGlobal, &tex, "mass:dirt2.bmp");
        
    gsKit_prim_quad_texture(gsGlobal, &tex,
                            310.0f,                   // x1
                            50.0f,                 // y1
                            0.0f,                  // u1
                            0.0f,                  // v1
                            
                            310.0f,                   // x2
                            250.0f,   // y2
                            0.0f,                  // u2
                            16.0f,

                            510.0f,   // x3
                            50.0f,                // y3
                            16.0f,            // u3
                            0.0f,                  // v3
                            
                            510.0f,   // x4
                            250.0f, // y4
                            16.0f,            // u4
                            16.0f,           // v4
                            2,TexCol);
    while(1)
    {
        gsKit_sync_flip(gsGlobal);
        gsKit_queue_exec(gsGlobal);
    }
    return 0;
}

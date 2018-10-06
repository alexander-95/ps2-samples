//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <stdio.h>
#include <malloc.h>
#include <gsKit.h>
#include <dmaKit.h>
#include <gsToolkit.h>

void gsKit_texture_abgr(GSGLOBAL* gsGlobal, GSTEXTURE* tex)
{
    u32 data[] = {0x000000FF, 0x0000FF00, 0x00FF0000, 0x00000000,
                  0x00000000, 0x000000FF, 0x0000FF00, 0x00FF0000,
                  0x00FF0000, 0x00000000, 0x000000FF, 0x0000FF00,
                  0x0000FF00, 0x00FF0000, 0xFF000000, 0x000000FF};
    
    tex->Width = 4;
    tex->Height = 4;
    tex->PSM = GS_PSM_CT32;
    tex->ClutPSM = 0;
    tex->TBW = 1;
    tex->Mem = data;
    tex->Clut = NULL;
    u32 VramTextureSize = gsKit_texture_size(tex->Width, tex->Height, tex->PSM);
    tex->Vram = gsKit_vram_alloc(gsGlobal, VramTextureSize, GSKIT_ALLOC_USERBUFFER);;
    tex->VramClut = 0;
    tex->Filter = GS_FILTER_NEAREST;
    tex->Delayed = 0;

    gsKit_texture_upload(gsGlobal, tex);
}

int main(int argc, char *argv[])
{
    GSGLOBAL *gsGlobal = gsKit_init_global();
    GSTEXTURE tex;
    u64 White = GS_SETREG_RGBAQ(0xFF,0xFF,0xFF,0x00,0x00);
    u64 Cyan = GS_SETREG_RGBAQ(0x00,0xFF,0xFF,0x00,0x00);
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);

    gsGlobal->PSM = GS_PSM_CT24;
    gsGlobal->PSMZ = GS_PSMZ_16S;
    gsGlobal->PrimAlphaEnable = GS_SETTING_ON;;
    
    dmaKit_init(D_CTRL_RELE_OFF,D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

    // Initialize the DMAC
    dmaKit_chan_init(DMA_CHANNEL_GIF);
    gsKit_init_screen(gsGlobal);
    gsKit_mode_switch(gsGlobal, GS_PERSISTENT);
    gsKit_clear(gsGlobal, Cyan);

    //gsKit_texture_bmp(gsGlobal, &tex, "mass:texture/test.bmp");
    gsKit_texture_abgr(gsGlobal, &tex);

    FILE* file = fopen("mass:texture/log.txt", "w");
    fprintf(file, "Width:   0x%x\n", tex.Width);
    fprintf(file, "Height:  0x%x\n", tex.Height);
    fprintf(file, "PSM:     0x%x\n", tex.PSM);
    fprintf(file, "ClutPSM: 0x%x\n", tex.ClutPSM);
    fprintf(file, "TBW:     0x%x\n", tex.TBW);
    fprintf(file, "Mem: 0x%x\n", tex.Mem);
    fprintf(file, "Clut: 0x%x\n", tex.Clut);
    fprintf(file, "Vram: 0x%x\n", tex.Vram);
    fprintf(file, "VramClut: 0x%x\n", tex.VramClut);
    fprintf(file, "Filter: 0x%x\n", tex.Filter);
    fprintf(file, "Delayed: 0x%x\n", tex.Delayed);

    
    
    fclose(file);
        
    gsKit_prim_quad_texture(gsGlobal, &tex,
                            0.0f, 0.0f,     // x1, y1
                            0.0f, 0.0f,     // u1, v1
                            
                            0.0f, 100.0f,   // x2, y2
                            0.0f, 4.0f,     // u2, y2

                            100.0f, 0.0f,   // x3, y3
                            4.0f, 0.0f,     // u3, v3
                            
                            100.0f, 100.0f, // x4, y4
                            4.0f, 4.0f,     // u4, v4
                            2,TexCol);
    while(1)
    {
        gsKit_sync_flip(gsGlobal);
        gsKit_queue_exec(gsGlobal);
    }
    return 0;
}

//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>
#include <dmaKit.h>
#include <malloc.h>
#include <stdio.h>
#include <math.h>

// rotate a 2d point about a centre point
void rotate(float* x, float* y, float theta, float cx, float cy)
{
    float tempx = *x - cx;
    float tempy = *y - cy;
    *x = (tempx*cosf(theta)) + (tempy*sinf(theta));
    *y = (tempy*cosf(theta)) - (tempx*sinf(theta));
    *x += cx;
    *y += cy;
}

void worldToCamera(float camera[3][3], float* x, float* y, float* z)
{
    float x1, y1, z1;
    x1 = camera[0][0]*(*x) + camera[0][1]*(*y) + camera[0][2]*(*z);
    y1 =                 0 + camera[1][1]*(*y) + camera[1][2]*(*z);
    z1 =                                                      (*z);
    *x = x1;
    *y = y1;
    *z = z1;
}

void perspectiveCorrection(float* x, float* y, float* z)
{
    float x1 = (*x) / (*z);
    float y1 = (*y) / (*z);
    *x = x1;
    *y = y1;
}

int main(int argc, char *argv[])
{
    u64 White, Red;
    GSGLOBAL *gsGlobal = gsKit_init_global();

    gsGlobal->PrimAlphaEnable = GS_SETTING_ON;

    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

    // Initialize the DMAC
    dmaKit_chan_init(DMA_CHANNEL_GIF);
    
    Red = GS_SETREG_RGBAQ(0xFF,0x00,0x00,0x00,0x00);
    White = GS_SETREG_RGBAQ(0xFF,0xFF,0xFF,0x00,0x00);
    
    gsKit_init_screen(gsGlobal);
    
    gsKit_mode_switch(gsGlobal, GS_PERSISTENT);
    gsKit_clear(gsGlobal, White);
    gsKit_mode_switch(gsGlobal, GS_ONESHOT);


    
    float xs[] = {300.0f, 300.0f, 400.0f};
    float ys[] = {200.0f, 350.0f, 350.0f};
    float zs[] = {500.0f, 500.0f, 500.0f};

    float x = 320;
    float y = 240;
    float s = 0;
    float f = 50;
    float camera[3][3] = {{f, s, x},
                          {0, f, y},
                          {0, 0, 1}};
    
    int i;
    for(i=0; i<3; i++)
    {
        worldToCamera(camera, &xs[i], &ys[i], &zs[i]);
        perspectiveCorrection(&xs[i], &ys[i], &zs[i]);
    }
    
    while(1)
    {
        //rotate(&xs[0], &ys[0], 0.1, xs[0], ys[0]);
        //rotate(&xs[1], &ys[1], 0.1, xs[0], ys[0]);
        //rotate(&xs[2], &ys[2], 0.1, xs[0], ys[0]);
        gsKit_prim_triangle(gsGlobal, xs[0], ys[0], xs[1], ys[1], xs[2], ys[2], 0, Red);
        printf("triangle\n");
        gsKit_queue_exec(gsGlobal);
        
        // Flip before exec to take advantage of DMA execution double buffering.
        gsKit_sync_flip(gsGlobal);
        
    }
    
    return 0;
}

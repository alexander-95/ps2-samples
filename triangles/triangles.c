//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>
#include <time.h>
#include <stdlib.h>

struct polygon
{
    float x1, y1, x2, y2, x3, y3;
    u64 color;
    char alpha;
    struct polygon* prev;
    struct polygon* next;
};

void drawRandomTriangle(GSGLOBAL* gsGlobal)
{
    struct polygon p1;
    p1.alpha = 127;
    p1.x1 = rand() % 640;
    p1.x2 = rand() % 640;
    p1.x3 = rand() % 640;
    p1.y1 = rand() % 512;
    p1.y2 = rand() % 512;
    p1.y3 = rand() % 512;
    while(p1.alpha > 64)
    {
	p1.alpha--;
	p1.color = GS_SETREG_RGBAQ(0x00, 0x00, 0xFF, p1.alpha, 0x00);
	gsKit_prim_triangle(gsGlobal, p1.x1, p1.y1, p1.x2, p1.y2, p1.x3, p1.y3, 0, p1.color);
	gsKit_queue_exec(gsGlobal);
	gsKit_sync_flip(gsGlobal);
    }
    p1.alpha = 127;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    GSGLOBAL* gsGlobal = gsKit_init_global();
    
    gsGlobal->PrimAlphaEnable = GS_SETTING_ON;

    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
		D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

    //Initialize the DMAC
    dmaKit_chan_init(DMA_CHANNEL_GIF);
    gsKit_init_screen(gsGlobal);
    
    u64 White = GS_SETREG_RGBAQ(0xFF,0xFF,0xFF,0x00,0x00);// set color
    gsKit_mode_switch(gsGlobal, GS_PERSISTENT);
    gsKit_clear(gsGlobal, White);
    gsKit_mode_switch(gsGlobal, GS_ONESHOT);

    while(1)
    {
	//change this to some controller input
	if(1)
	{
	    drawRandomTriangle(gsGlobal);
	}
    }
    return 0;
}

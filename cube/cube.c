//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <kernel.h>
#include <gsKit.h>
#include <dmaKit.h>
#include <gsToolkit.h>
#include <malloc.h>
#include <stdio.h>
#include <math.h>
#include <sifrpc.h>
#include <libpad.h>
#include "controller.h"


struct camera
{
    //intrinsic parameters
    float x, y; // principle point
    float s; // skew coefficient
    float f; // focal length
    float mat[3][3];
    float pos[3];
    float rot[3];
};

void setupCamera(struct camera* cam)
{
    cam->x = 320.0f;
    cam->y = 256.0f;
    cam->s = 0.0f;
    cam->f = 300.0f;
    float matrix[3][3] = {{cam->f, cam->s, cam->x},
                          {     0, cam->f, cam->y},
                          {     0,      0,      1}};
    int i,j;
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)cam->mat[i][j] = matrix[i][j];
        cam->pos[i] = 0;
        cam->rot[i] = 0;
    }
}

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

// rotate a 3d point around the x axis of the camera
void rotateX(struct camera* cam, float* x, float* y, float* z, float theta)
{
    float tempx = *x - cam->pos[0];
    float tempy = *y - cam->pos[1];
    float tempz = *z - cam->pos[2];
    *x = tempx;
    *y = cosf(theta)*tempy - sinf(theta)*tempz;
    *z = sinf(theta)*tempy + cosf(theta)*tempz;
    *x += cam->pos[0];
    *y += cam->pos[1];
    *z += cam->pos[2];
}

// rotate a 3d point around the y axis of the camera
void rotateY(struct camera* cam, float* x, float* y, float* z, float theta)
{
    float tempx = *x - cam->pos[0];
    float tempy = *y - cam->pos[1];
    float tempz = *z - cam->pos[2];
    *x = cosf(theta)*tempx + sinf(theta)*tempz;
    *y = tempy;
    *z = -sinf(theta)*tempx + cosf(theta)*tempz;
    *x += cam->pos[0];
    *y += cam->pos[1];
    *z += cam->pos[2];
}

struct square
{
    float x[4];
    float y[4];
    float z[4];
    u64 color;
};

void squareRotateY(struct camera* cam, struct square* s, float theta)
{
    int i;
    for(i=0;i<4;i++) rotateY(cam, &s->x[i], &s->y[i], &s->z[i], theta);
}

void drawSquare(GSGLOBAL* gsGlobal, struct camera cam, struct square s)
{
    int i;
    // frustum culling check(need to account for camera rotation)
    /*for(i=0;i<4;i++)
    {
        if(s.x[i]/s.z[i] < cam.x/cam.f*(-1) || s.x[i]/s.z[i] > cam.x/cam.f)return;
        if(s.y[i]/s.z[i] < cam.y/cam.f*(-1) || s.y[i]/s.z[i] > cam.y/cam.f)return;
        }*/

    // camera translation
    /*for(i=0;i<4;i++)
    {
        s.x[i] -= cam.pos[0];
        s.y[i] -= cam.pos[1];
        s.z[i] -= cam.pos[2];
	}*/

    //camera rotation
    squareRotateY(&cam, &s, cam.rot[1]);

    //camera transformation and perspective correction
    float x[4], y[4], z[4];
    for(i=0;i<4;i++)
    {
      x[i] = cam.mat[0][0]*(s.x[i]-cam.pos[0]) + cam.mat[0][1]*(s.y[i]-cam.pos[1]) + cam.mat[0][2]*(s.z[i]-cam.pos[2]);
        y[i] =                                 0 + cam.mat[1][1]*(s.y[i]-cam.pos[1]) + cam.mat[1][2]*(s.z[i]-cam.pos[2]);
        z[i] =                                                                                       (s.z[i]-cam.pos[2]);
        
      x[i] /= z[i];
      y[i] /= z[i];
    }
    // calculate the center of the quad
    // get the distance from the center to the camera as the z value.
    float cx = (s.x[0] + s.x[1] + s.x[2] + s.x[3])/4;
    float cy = (s.y[0] + s.y[1] + s.y[2] + s.y[3])/4;
    float cz = (s.z[0] + s.z[1] + s.z[2] + s.z[3])/4;
    float distance = sqrtf(powf((cx - cam.pos[0]),2) +
                           powf((cy - cam.pos[1]),2) +
                           powf((cz - cam.pos[2]),2));
    
    gsKit_prim_quad(gsGlobal, x[0], y[0], x[1], y[1], x[2], y[2], x[3], y[3], (int)distance*(-1), s.color);
}

struct cuboid
{
    struct square s[6];
};

void createCuboid(struct cuboid* c, float x1, float y1, float z1, float x2, float y2, float z2)
{
    u64 R = GS_SETREG_RGBAQ(0xFF,0x00,0x00,0x00,0x00);
    u64 G = GS_SETREG_RGBAQ(0x00,0xFF,0x00,0x00,0x00);
    u64 B = GS_SETREG_RGBAQ(0x00,0x00,0xFF,0x00,0x00);
    u64 C = GS_SETREG_RGBAQ(0x00,0xFF,0xFF,0x00,0x00);
    u64 M = GS_SETREG_RGBAQ(0xFF,0x00,0xFF,0x00,0x00);
    u64 Y = GS_SETREG_RGBAQ(0xFF,0xFF,0x00,0x00,0x00);

    struct square* s1 = &c->s[0];
    struct square* s2 = &c->s[1];
    struct square* s3 = &c->s[2];
    struct square* s4 = &c->s[3];
    struct square* s5 = &c->s[4];
    struct square* s6 = &c->s[5];
    
    s1->x[0] = x1; s1->x[1] = x1; s1->x[2] = x2; s1->x[3] = x2;
    s1->y[0] = y1; s1->y[1] = y2; s1->y[2] = y1; s1->y[3] = y2;
    s1->z[0] = z1; s1->z[1] = z1; s1->z[2] = z1; s1->z[3] = z1;

    s2->x[0] = x1; s2->x[1] = x1; s2->x[2] = x2; s2->x[3] = x2;
    s2->y[0] = y1; s2->y[1] = y2; s2->y[2] = y1; s2->y[3] = y2;
    s2->z[0] = z2; s2->z[1] = z2; s2->z[2] = z2; s2->z[3] = z2;

    s3->x[0] = x1; s3->x[1] = x1; s3->x[2] = x1; s3->x[3] = x1;
    s3->y[0] = y1; s3->y[1] = y2; s3->y[2] = y1; s3->y[3] = y2;
    s3->z[0] = z1; s3->z[1] = z1; s3->z[2] = z2; s3->z[3] = z2;

    s4->x[0] = x2; s4->x[1] = x2; s4->x[2] = x2; s4->x[3] = x2;
    s4->y[0] = y1; s4->y[1] = y2; s4->y[2] = y1; s4->y[3] = y2;
    s4->z[0] = z1; s4->z[1] = z1; s4->z[2] = z2; s4->z[3] = z2;

    s5->x[0] = x1; s5->x[1] = x1; s5->x[2] = x2; s5->x[3] = x2;
    s5->y[0] = y1; s5->y[1] = y1; s5->y[2] = y1; s5->y[3] = y1;
    s5->z[0] = z1; s5->z[1] = z2; s5->z[2] = z1; s5->z[3] = z2;

    s6->x[0] = x1; s6->x[1] = x1; s6->x[2] = x2; s6->x[3] = x2;
    s6->y[0] = y2; s6->y[1] = y2; s6->y[2] = y2; s6->y[3] = y2;
    s6->z[0] = z1; s6->z[1] = z2; s6->z[2] = z1; s6->z[3] = z2;

    s1->color = R;
    s2->color = G;
    s3->color = B;
    s4->color = C;
    s5->color = M;
    s6->color = Y;    
}

void drawCuboid(GSGLOBAL* gsGlobal, struct camera cam, struct cuboid* c)
{
    int i;
    for(i=0;i<6;i++)drawSquare(gsGlobal, cam, c->s[i]);
}
  
int main(int argc, char *argv[])
{
    u64 White;
    GSGLOBAL *gsGlobal = gsKit_init_global();
    gsGlobal->PrimAlphaEnable = GS_SETTING_ON;

    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

    
    // Initialize the DMAC
    dmaKit_chan_init(DMA_CHANNEL_GIF);
    White = GS_SETREG_RGBAQ(0xFF,0xFF,0xFF,0x00,0x00);
    
    gsKit_init_screen(gsGlobal);
    
    gsKit_mode_switch(gsGlobal, GS_PERSISTENT);
    gsKit_clear(gsGlobal, White);
    gsKit_mode_switch(gsGlobal, GS_ONESHOT);
    
    // controller setup
    static int padBuf[256] __attribute__((aligned(64)));
    unsigned int old_pad = 0;
    unsigned int new_pad, paddata;
    int ret=0, port=0, slot=0;
    struct padButtonStatus buttons;
    SifInitRpc(0);
    
    loadModules();
    
    padInit(0);
    if((ret = padPortOpen(port, slot, padBuf)) == 0)
    {
        printf("padOpenPort failed: %d\n", ret);
        SleepThread();
    }
        
    if(!initializePad(port, slot))
    {
        printf("pad initalization failed!\n");
        SleepThread();
    }
    struct camera cam;
    setupCamera(&cam);
    struct cuboid c;
    createCuboid(&c, 100, 500, 10200, 1100, 1500, 11200);
    while(1)
    {
        drawCuboid(gsGlobal, cam, &c);
        printf("cube\n");

        ret=padGetState(port, slot);
        while((ret != PAD_STATE_STABLE) && (ret != PAD_STATE_FINDCTP1))
        {
            if(ret==PAD_STATE_DISCONN) printf("Pad(%d, %d) is disconnected\n", port, slot);
            ret = padGetState(port, slot);
        }
        ret = padRead(port, slot, &buttons);
        if(ret != 0)
        {
            paddata = 0xffff ^ buttons.btns;
            new_pad = paddata & ~old_pad;
            old_pad = paddata;

            // react to controller input
            if(old_pad & PAD_LEFT)
	      {
		cam.pos[0]-=(20*cosf(cam.rot[1]));
		cam.pos[2]-=(20*sinf(cam.rot[1]));
	      }
            if(old_pad & PAD_RIGHT)
	      {
		cam.pos[0]+=(20*cosf(cam.rot[1]));
		cam.pos[2]+=(20*sinf(cam.rot[1]));
	      }
	    if(old_pad & PAD_UP)
	      {
		cam.pos[0]-=(20*sinf(cam.rot[1]));
		cam.pos[2]+=(20*cosf(cam.rot[1]));
	      }
	    if(old_pad & PAD_DOWN)
	      {
		cam.pos[0]+=(20*sinf(cam.rot[1]));
		cam.pos[2]-=(20*cosf(cam.rot[1]));
	      }
            if(old_pad & PAD_TRIANGLE) cam.pos[1]-=(20);
            if(old_pad & PAD_CROSS)    cam.pos[1]+=(20);
            if(old_pad & PAD_SQUARE)   cam.rot[1]+=0.01;
            if(old_pad & PAD_CIRCLE)   cam.rot[1]-=0.01;
            
        }

        //cam.pos[2]+=10;
        //cam.rot[1]+=0.01;
        // Flip before exec to take advantage of DMA execution double buffering.
        gsKit_sync_flip(gsGlobal);
        gsKit_queue_exec(gsGlobal);        
    }
    return 0;
}

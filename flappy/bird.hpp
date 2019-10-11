//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>

class Bird
{
public:
    int x;
    int y;
    int color;
    float vy;
    unsigned char cycle;
    GSGLOBAL* gsGlobal;
    GSTEXTURE* spritesheet;

    Bird();
    ~Bird();
    void draw();
    void reset(int color);
    int touchingGround();
    //int collision(struct pipeList* pipes);
    
};

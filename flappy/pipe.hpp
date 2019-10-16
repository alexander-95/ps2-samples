//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>

class Pipe
{
public:
    int x;
    int y;
    int d;
    int nightMode;
    Pipe* next;
    Pipe* prev;
    GSGLOBAL* gsGlobal;
    GSTEXTURE* spritesheet;

    Pipe();
    ~Pipe();
    void print();
};

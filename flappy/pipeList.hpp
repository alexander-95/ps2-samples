//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>
#include "pipe.hpp"

class PipeList
{
public:
    Pipe* head;
    Pipe* tail;
    int length;
    int gap;
    int* nightMode;
    GSGLOBAL* gsGlobal;
    GSTEXTURE* spritesheet;

    PipeList();
    ~PipeList();
    void draw();
    void setup();
    void reset();
};

//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "pipeList.hpp"


PipeList::PipeList()
{

}

PipeList::~PipeList()
{

}

void PipeList::draw()
{

    int offset = 0;
    if(nightMode)offset = 26;
    Pipe* curr = head;
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    while(curr!=NULL)
    {
        //upper pipe
        gsKit_prim_quad_texture(gsGlobal, spritesheet,
                                curr->x, curr->y-74,         // x2, y2
                                0.0f+offset, 13.0f,          // u2, v2

                                curr->x, curr->y-50,          // x1, y1
                                0.0f+offset, 0.0f,            // u1, v1

                                curr->x+curr->d, curr->y-74, // x4, y4
                                26.0f+offset, 13.0f,         // u4, v4

                                curr->x+curr->d, curr->y-50,  // x3, y3
                                26.0f+offset, 0.0f,           // u3, v3

                                1, TexCol);
        gsKit_prim_quad_texture(gsGlobal, spritesheet,
                                curr->x, curr->y-450,         // x2, y2
                                0.0f+offset, 14.0f,          // u2, v2

                                curr->x, curr->y-74,          // x1, y1
                                0.0f+offset, 13.0f,            // u1, v1

                                curr->x+curr->d, curr->y-450, // x4, y4
                                26.0f+offset, 14.0f,         // u4, v4

                                curr->x+curr->d, curr->y-74,  // x3, y3
                                26.0f+offset, 13.0f,           // u3, v3
                                1, TexCol);
        
        // lower pipe
        gsKit_prim_quad_texture(gsGlobal, spritesheet,
                                curr->x, curr->y+50,          // x1, y1
                                0.0f+offset, 0.0f,            // u1, v1

                                curr->x, curr->y+74,         // x2, y2
                                0.0f+offset, 13.0f,          // u2, v2

                                curr->x+curr->d, curr->y+50,  // x3, y3
                                26.0f+offset, 0.0f,           // u3, v3

                                curr->x+curr->d, curr->y+74, // x4, y4
                                26.0f+offset, 13.0f,         // u4, v4
                                1, TexCol);
        
        gsKit_prim_quad_texture(gsGlobal, spritesheet,
                                curr->x, curr->y+74,          // x1, y1
                                0.0f+offset, 13.0f,            // u1, v1

                                curr->x, curr->y+450,         // x2, y2
                                0.0f+offset, 14.0f,          // u2, v2

                                curr->x+curr->d, curr->y+74,  // x3, y3
                                26.0f+offset, 13.0f,           // u3, v3

                                curr->x+curr->d, curr->y+450, // x4, y4
                                26.0f+offset, 14.0f,         // u4, v4
                                1, TexCol);
        curr = curr->next;
    }
    return;
}

void PipeList::setup()
{
    gap = 200;
    Pipe* curr = (Pipe*) malloc(sizeof(Pipe));
    head = curr;
    length = 4;
    for(int i=0; i<length-1; i++)
    {
        Pipe* p = (Pipe*) malloc(sizeof(Pipe));
        curr->next = p;
        p->prev = curr;
        curr->x = 640+gap*(i), curr->y = rand() % 300 + 50, curr->d = 52;
        curr = curr->next;
    }
    curr->next = NULL;
}

void PipeList::reset()
{
    Pipe* curr = head;
    for(int i=0; i<length; i++)
    {
        curr->x = 640+gap*(i), curr->y = rand() % 300 + 50, curr->d = 52;
        curr = curr->next;
    }
}

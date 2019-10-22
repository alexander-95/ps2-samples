//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <stdio.h>

#include "pipe.hpp"

Pipe::Pipe()
{

}

Pipe::~Pipe()
{

}

void Pipe::print()
{
    printf("x           = %d\n", x);
    printf("y           = %d\n", y);
    printf("d           = %d\n", d);
    printf("nightMode   = %d\n", nightMode);
    printf("next        = %p\n", next);
    printf("prev        = %p\n", prev);
    printf("gsGlobal    = %p\n", gsGlobal);
    printf("spritesheet = %p\n\n", spritesheet);
}

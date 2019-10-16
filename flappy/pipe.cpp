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
    printf("next        = %x\n", next);
    printf("prev        = %x\n", prev);
    printf("gsGlobal    = %x\n", gsGlobal);
    printf("spritesheet = %x\n\n", spritesheet);
}

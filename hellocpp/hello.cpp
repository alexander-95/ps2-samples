//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <stdio.h>
#include "hello.h"

hello::hello()
{
    printf("Object Constructed\n");
}

hello::~hello()
{
    printf("Object Destructed\n");
}

void hello::print()
{
    printf("hello world\n");
}

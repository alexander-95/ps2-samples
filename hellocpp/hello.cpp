//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <stdio.h>

class hello
{
public:
    hello();
    ~hello();
    void print();
};

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

int main()
{
    hello h;
    h.print();
    return 1;
}

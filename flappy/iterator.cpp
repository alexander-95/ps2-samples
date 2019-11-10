//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include "iterator.hpp"
#include "debugMenu.hpp"

MenuIter::MenuIter(const DebugMenu* m)
{
    menu = m;
}

MenuIter::~MenuIter()
{

}

void MenuIter::first()
{
    index = 0;
}

void MenuIter::next()
{
    index++;
}

int MenuIter::isDone()
{
    return index == menu->itemCount;
}

menuItem* MenuIter::currentItem()
{
    return &menu->item[index];
}

//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

struct menuItem;
class DebugMenu;

#ifndef ITERATOR_H
#define ITERATOR_H

class MenuIter
{
public:
    const DebugMenu* menu;
    int index;

    MenuIter(const DebugMenu* m);
    ~MenuIter();
    void first();
    void next();
    int isDone();
    menuItem* currentItem();
};

#endif

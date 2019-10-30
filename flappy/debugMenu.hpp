//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>
#include "log.hpp"

class DebugMenu
{
public:
    int x;
    int y;
    int w;
    int h;
    int topLeft;
    int topRight;
    int bottomLeft;
    int bottomRight;
    char* title;
    int cursor;
    u8 itemCount;
    u8 active;
    struct menuItem* item;
    Log* l;

    DebugMenu(Log* l, char* title);
    ~DebugMenu();
    void draw();
    void clear();
    void setCursor();
    void refreshLabel();
    struct menuItem* currentItem();
    void nextValue();
    void prevValue();
    void nextItem();
    void prevItem();
};

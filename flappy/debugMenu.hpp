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

    ~DebugMenu();
    void draw();
    void clear();
    void setCursor();
    void refreshLabel();
    static DebugMenu& getInstance(Log* l, char* title);
    struct menuItem* currentItem();
    void nextValue();
    void prevValue();
    void nextItem();
    void prevItem();

private:
    DebugMenu(Log* l, char* title);
    static DebugMenu* instance;
};

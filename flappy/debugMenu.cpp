//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include "debugMenu.hpp"
#include "iterator.hpp"

DebugMenu& DebugMenu::getInstance(Log* l, char* title)
{
    static DebugMenu instance(l, title);
    return instance;
}

DebugMenu::DebugMenu(Log* l, char* title)
{
    this->l = l;
    this->title = title;
    
    x = 45;
    y = 10;
    w = 30;
    h = 20;
    cursor = 0;
    
    // corners of the debug menu
    topLeft = y*l->bufWidth + x;
    topRight = y*l->bufWidth + (x+w);
    bottomLeft = (y+h)*l->bufWidth + x;
    bottomRight = (y+h)*l->bufWidth + (x+w);
}

DebugMenu::~DebugMenu()
{

}

void DebugMenu::draw()
{
    // temporary counters
    int i, j;

    u8 borderColor = 3; // blue
    u8 cursorColor = 4; // cyan

    // set the corners of the debug menu
    l->buffer[topLeft] = 127;
    l->buffer[topRight] = 128;
    l->buffer[bottomLeft] = 129;
    l->buffer[bottomRight] = 130;
    l->color[topLeft] = borderColor;
    l->color[topRight] = borderColor;
    l->color[bottomLeft] = borderColor;
    l->color[bottomRight] = borderColor;

    // set the borders of the debug menu
    for(i = topLeft+1; i < topRight; i++)
    {
         l->buffer[i] = 137;
         l->color[i] = borderColor;
    }
    for(i = bottomLeft+1; i < bottomRight; i++)
    {
        l->buffer[i] = 137;
        l->color[i] = borderColor;
    }
    for(i = topLeft+l->bufWidth; i < bottomLeft; i+=l->bufWidth)
    {
        l->buffer[i] = 136;
        l->color[i] = borderColor;
    }
    for(i = topRight+l->bufWidth; i < bottomRight; i+=l->bufWidth)
    {
        l->buffer[i] = 136;
        l->color[i] = borderColor;
    }

    // set the title bar of the debug menu
    for(i = 0; title[i]; i++) l->buffer[topLeft+l->bufWidth+i+2] = title[i];
    for(i = topLeft+(2*l->bufWidth)+1; i < topRight+(2*l->bufWidth); i++)
    {
         l->buffer[i] = 137;
         l->color[i] = borderColor;
    }
    l->buffer[topLeft+(2*l->bufWidth)] = 131;
    l->buffer[topRight+(2*l->bufWidth)] = 132;
    l->color[topLeft+(2*l->bufWidth)] = borderColor;
    l->color[topRight+(2*l->bufWidth)] = borderColor;
    
    // set the cursor location
    l->buffer[topLeft+2+((3+cursor)*l->bufWidth)] = '>';
    l->color[topLeft+2+((3+cursor)*l->bufWidth)] = cursorColor;

    MenuIter* iter = createIterator();
    
    // fill in the menu items
    for(iter->first(); !iter->isDone();iter->next())
    {
        // fill out the menu item names
        for(j = 0; iter->currentItem()->name[j]; j++)
        {
            l->buffer[topLeft+(l->bufWidth*(3+iter->index))+j+4] = iter->currentItem()->name[j];
        }
        if(iter->currentItem()->label)
        {
            for(j = 0; iter->currentItem()->label[iter->currentItem()->val][j]; j++)
            {
                l->buffer[topLeft+(l->bufWidth*(3+iter->index))+j+14] = iter->currentItem()->label[iter->currentItem()->val][j];
            }
        }
        else
        {
            char label[3];
            sprintf(label, "%d", iter->currentItem()->val);
            for(j = 0; label[j]; j++) l->buffer[topLeft+(l->bufWidth*(3+iter->index))+j+14] = label[j];
        }
    }
    active = 1;
}

void DebugMenu::clear()
{
    for(int i = y; i <= y+h; i++)
    {
        for(int j = x; j <= x+w; j++) l->buffer[(i*l->bufWidth)+j] = ' ';
    }
    active = 0;
}

void DebugMenu::setCursor()
{
    // clear entire cursor column
    for(int i = 0; i < h-3; i++)
    {
        int curr = (x+2)+((y+3+i)*l->bufWidth);
        l->buffer[curr] = ' ';
        l->color[curr] = 7;
    }

    // draw the new cursor
    int curr = (y*l->bufWidth+x)+2+((3+cursor)*l->bufWidth);
    l->buffer[curr] = '>';
    l->color[curr] = 4;
}

void DebugMenu::refreshLabel()
{
    for(int i = 0; i < (w-6)/2; i++)l->buffer[(x+y*l->bufWidth) + l->bufWidth*(3+cursor) + 14+i] = ' ';
    if(item[cursor].label)
    {
        for(int j = 0; item[cursor].label[item[cursor].val][j]; j++) l->buffer[(x+y*l->bufWidth)+(l->bufWidth*(3+cursor))+j+14] = item[cursor].label[item[cursor].val][j];
    }
    else
    {
        char label[3];
        sprintf(label, "%d", item[cursor].val);
        for(int j = 0; label[j]; j++) l->buffer[(x+y*l->bufWidth)+(l->bufWidth*(3+cursor))+j+14] = label[j];
    }
}

struct menuItem* DebugMenu::currentItem()
{
    return &item[cursor];
}

void DebugMenu::nextValue()
{
    item[cursor].val++;
    if(item[cursor].val > item[cursor].max) item[cursor].val = item[cursor].min;
    refreshLabel();
}

void DebugMenu::prevValue()
{
    item[cursor].val--;
    if(item[cursor].val < item[cursor].min) item[cursor].val = item[cursor].max;
    refreshLabel();
}

void DebugMenu::nextItem()
{
    cursor++;
    if(cursor == itemCount) cursor = 0;
    setCursor();
}

void DebugMenu::prevItem()
{
    cursor--;
    if(cursor == -1) cursor = itemCount-1;
    setCursor();
}

MenuIter* DebugMenu::createIterator()
{
    return new MenuIter(this);
}

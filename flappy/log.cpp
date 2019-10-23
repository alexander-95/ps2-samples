#include "log.hpp"
#include "graphics.hpp"

void logMessage(GSGLOBAL* gsGlobal, GSTEXTURE* font, struct log* l, char* msg)
{

    int bufWidth = 90, bufHeight = 56;
    if(l->logToFile)
    {
        FILE* f = fopen(l->logfile, "a");
        fprintf(f, "%s\n",msg);
        fclose(f);
    }
    if(l->logToScreen)
    {
        printf("%s", msg);
        clearLine(l->buffer, l->index);
        setLine(l->buffer, l->index, msg);
        (l->index)++;// = (l->index+1) % 56;
        if(l->index == bufHeight)
        {
            l->wrap = 1;
            l->index = 0;
        }
        printf("index = %d\n", l->index);
        //updateFrame(gsGlobal, font, l->buffer);
    }
}

void setLine(char* buffer, int lineNumber, char* str)
{
    int bufWidth = 90, bufHeight = 56;
    for(int i = 0; str[i]; i++)
    {
        buffer[i+lineNumber*bufWidth] = str[i];
        printf("%c",str[i]);
    }
    printf("\n");
}

void clearLine(char* buffer, int lineNumber)
{
    int bufWidth = 90, bufHeight = 56;
    for(int i = 0; i < bufWidth; i++)
    {
        buffer[i+lineNumber*bufWidth] = 0;
    }
}

void clearBuffer(struct log* l)
{
    int bufWidth = 90, bufHeight = 56;
    for(int i = 0; i < bufHeight; i++)
    {
        clearLine(l->buffer, i);
    }
    l->index = 0;
    l->wrap = 0;
}

void drawMenu(struct log* l, int x, int y, int w, int h, char* title, int cursor, struct menuItem* item)
{
    // variables related to the buffer
    int bufWidth = 90, bufHeight = 56;

    // temporary counters
    int i, j;

    // corners of the debug menu
    int topLeft, topRight, bottomLeft, bottomRight;
    
    topLeft = y*bufWidth + x;
    topRight = y*bufWidth + (x+w);
    bottomLeft = (y+h)*bufWidth + x;
    bottomRight = (y+h)*bufWidth + (x+w);

    // set the corners of the debug menu
    l->buffer[topLeft] = '+';
    l->buffer[topRight] = '+';
    l->buffer[bottomLeft] = '+';
    l->buffer[bottomRight] = '+';

    // set the borders of the debug menu
    for(i = topLeft+1; i < topRight; i++) l->buffer[i] = '-';
    for(i = bottomLeft+1; i < bottomRight; i++) l->buffer[i] = '-';
    for(i = topLeft+bufWidth; i < bottomLeft; i+=bufWidth) l->buffer[i] = '|';
    for(i = topRight+bufWidth; i < bottomRight; i+=bufWidth) l->buffer[i] = '|';

    // set the title bar of the debug menu
    for(i = 0; title[i]; i++) l->buffer[topLeft+bufWidth+i+2] = title[i];
    for(i = topLeft+(2*bufWidth)+1; i < topRight+(2*bufWidth); i++) l->buffer[i] = '-';
    l->buffer[topLeft+(2*bufWidth)] = '+';
    l->buffer[topRight+(2*bufWidth)] = '+';
    
    // set the cursor location
    l->buffer[topLeft+2+((3+cursor)*bufWidth)] = '>';
    
    // fill in the menu items
    for(i = 0; i < 3; i++)
    {
        // fill out the menu item names
        for(j = 0; item[i].name[j]; j++) l->buffer[topLeft+(bufWidth*(3+i))+j+4] = item[i].name[j];

        if(item[i].label)
        {
            for(j = 0; item[i].label[item[i].val][j]; j++) l->buffer[topLeft+(bufWidth*(3+i))+j+14] = item[i].label[item[i].val][j];
        }
        else
        {
            char label[3];
            sprintf(label, "%d", item[i].val);
            for(j = 0; label[j]; j++) l->buffer[topLeft+(bufWidth*(3+i))+j+14] = label[j];
        }
    }
    
    
}

void clearMenu(struct log* l, int x, int y, int w, int h)
{
    // variables related to the buffer
    int bufWidth = 90, bufHeight = 56;

    // temporary counters
    int i, j;

    // corners of the debug menu
    int topLeft, topRight, bottomLeft, bottomRight;
    
    topLeft = y*bufWidth + x;
    topRight = y*bufWidth + (x+w);
    bottomLeft = (y+h)*bufWidth + x;
    bottomRight = (y+h)*bufWidth + (x+w);

    
    for(i = y; i <= y+h; i++)
    {
        for(j = x; j <= x+w; j++) l->buffer[(i*bufWidth)+j] = ' ';
    }
}

void setCursor(struct log* l, int x, int y, int w, int h, int cursor)
{
    // variables related to the buffer
    int bufWidth = 90, bufHeight = 56;
    
    // clear entire cursor column
    int i;
    for(int i = 0; i < h-3; i++)
    {
        l->buffer[(x+2)+((y+3+i)*bufWidth)] = ' ';
    }

    // draw the new cursor
    l->buffer[(y*bufWidth+x)+2+((3+cursor)*bufWidth)] = '>';
}

void refreshLabel(struct log* l, int x, int y, int w, int h, int cursor, struct menuItem* item)
{
    // variables related to the buffer
    int bufWidth = 90, bufHeight = 56;
    
    int i, j;
    for(i = 0; i < (w-6)/2; i++)l->buffer[(x+y*bufWidth) + bufWidth*(3+cursor) + 14+i] = ' ';
    if(item[cursor].label)
    {
        for(j = 0; item[cursor].label[item[cursor].val][j]; j++) l->buffer[(x+y*bufWidth)+(bufWidth*(3+cursor))+j+14] = item[cursor].label[item[cursor].val][j];
    }
    else
    {
        char label[3];
        sprintf(label, "%d", item[cursor].val);
        for(j = 0; label[j]; j++) l->buffer[(x+y*bufWidth)+(bufWidth*(3+cursor))+j+14] = label[j];
    }


}

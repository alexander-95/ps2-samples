#include "log.h"
#include "graphics.h"

void logMessage(GSGLOBAL* gsGlobal, GSTEXTURE* font, struct log* l, char* msg)
{
    setLine(l->buffer, l->index, msg);
    l->index = (l->index++) % 28;
    updateFrame(gsGlobal, font, l->buffer);
}

void setLine(char* buffer, int lineNumber, char* str)
{
    int i;
    for(i = 0; str[i]; i++)
    {
        buffer[i+lineNumber*45] = str[i];
        printf("%c\n",str[i]);
    }
}

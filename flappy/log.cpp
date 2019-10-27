#include "log.hpp"
#include "graphics.hpp"

void logMessage(GSGLOBAL* gsGlobal, GSTEXTURE* font, struct log* l, char* msg)
{

    if(l->logToFile)
    {
        FILE* f = fopen(l->logfile, "a");
        fprintf(f, "%s\n",msg);
        fclose(f);
    }
    if(l->logToScreen)
    {
        printf("%s", msg);
        clearLine(l, l->index);
        setLine(l, l->index, msg);
        (l->index)++;// = (l->index+1) % 56;
        if(l->index == l->bufHeight)
        {
            l->wrap = 1;
            l->index = 0;
        }
        printf("index = %d\n", l->index);
        //updateFrame(gsGlobal, font, l->buffer);
    }
}

void setLine(struct log* l, int lineNumber, char* str)
{
    for(int i = 0; str[i]; i++)
    {
        l->buffer[i+lineNumber*l->bufWidth] = str[i];
        printf("%c",str[i]);
    }
    printf("\n");
}

void clearLine(struct log* l, int lineNumber)
{
    for(int i = 0; i < l->bufWidth; i++)
    {
        l->buffer[i+lineNumber*l->bufWidth] = 0;
    }
}

void clearBuffer(struct log* l)
{
    for(int i = 0; i < l->bufHeight; i++)
    {
        clearLine(l, i);
    }
    l->index = 0;
    l->wrap = 0;
}

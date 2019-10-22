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
        clearLine(l->buffer, l->index);
        setLine(l->buffer, l->index, msg);
        (l->index)++;// = (l->index+1) % 56;
        if(l->index == 56)
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
    for(int i = 0; str[i]; i++)
    {
        buffer[i+lineNumber*90] = str[i];
        printf("%c",str[i]);
    }
    printf("\n");
}

void clearLine(char* buffer, int lineNumber)
{
    for(int i = 0; i < 90; i++)
    {
        buffer[i+lineNumber*90] = 0;
    }
}

void clearBuffer(struct log* l)
{
    for(int i = 0; i < 56; i++)
    {
        clearLine(l->buffer, i);
    }
    l->index = 0;
    l->wrap = 0;
}

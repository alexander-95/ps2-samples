#include "log.h"
#include "graphics.h"

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
        setLine(l->buffer, l->index, msg);
        l->index = (l->index++) % 56;
        updateFrame(gsGlobal, font, l->buffer);
    }
}

void setLine(char* buffer, int lineNumber, char* str)
{
    int i;
    for(i = 0; str[i]; i++)
    {
        buffer[i+lineNumber*90] = str[i];
        printf("%c",str[i]);
    }
    printf("\n");
}

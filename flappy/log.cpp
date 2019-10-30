#include "log.hpp"
#include "graphics.hpp"

Log::Log()
{}

Log::~Log()
{}

void Log::logMessage(char* msg)
{
    if(logToFile)
    {
        FILE* f = fopen(logfile, "a");
        fprintf(f, "%s\n",msg);
        fclose(f);
    }
    if(logToScreen)
    {
        printf("%s", msg);
        clearLine(index);
        setLine(index, msg);
        index++;// = (l->index+1) % 56;
        if(index == bufHeight)
        {
            wrap = 1;
            index = 0;
        }
        printf("index = %d\n", index);
        //updateFrame(gsGlobal, font, l->buffer);
    }
}

void Log::setLine(int lineNumber, char* msg)
{
    for(int i = 0; msg[i]; i++)
    {
        buffer[i+lineNumber*bufWidth] = msg[i];
        printf("%c", msg[i]);
    }
    printf("\n");
}

void Log::clearLine(int lineNumber)
{
    for(int i = 0; i < bufWidth; i++)
    {
        buffer[i+lineNumber*bufWidth] = 0;
    }
}

void Log::clearBuffer()
{
    for(int i = 0; i < bufHeight; i++)
    {
        clearLine(i);
    }
    index = 0;
    wrap = 0;
}

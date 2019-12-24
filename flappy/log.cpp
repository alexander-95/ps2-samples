#include "log.hpp"
#include "graphics.hpp"

Log::Log(char* logfile)
{
    init();
    if(logfile)
    {
        FILE* f = fopen(logfile, "w");
        fprintf(f, "log initialised\n");
        fclose(f);
    }
}

Log::Log()
{
    init();
}

void Log::init()
{
    bufWidth = 90; // magic number
    bufHeight = 56; // magic number
    index = 0;
    buffer = new char[bufWidth*bufHeight];
    color = new u8[bufWidth*bufHeight];
    logfile = NULL;
    logToScreen = 1;
    clearBuffer();
}

Log::~Log()
{
    delete buffer;
}

void Log::logToFile(char* msg)
{
    FILE* f = fopen(logfile, "a");
    fprintf(f, "%s\n",msg);
    fclose(f);
}

void Log::logMessage(char* msg)
{
    if(logfile)
    {
        logToFile(msg);
    }
    if(logToScreen)
    {
        printf("%s", msg);
        clearLine(index);
        for(int i = 0; msg[i]; i++)
        {
            int curr = i+index*bufWidth;
            buffer[curr] = msg[i];
            color[curr] = 7;
        }
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

void Log::debug(char* msg, ...)
{
    char buf[1024];
    va_list args;
    int printed;

    va_start(args, msg);
    printed = vsprintf(buf, msg, args);
    va_end(args);

    if(logfile) logToFile(buf);
    if(logToScreen)
    {
        printf("%s", buf);
        clearLine(index);
        char* label = "DEBUG: "; // length = 7
        for(int i = 0; label[i]; i++)
        {
            int curr = i+index*bufWidth;
            buffer[curr] = label[i];
            color[curr] = 2; // green
        }
        for(int i = 0; buf[i]; i++)
        {
            int curr = (i+7)+index*bufWidth;
            buffer[curr] = buf[i];
            color[curr] = 7;
        }
        index++;
        if(index == bufHeight)
        {
            wrap = 1;
            index = 0;
        }
    }
}

void Log::clearLine(int lineNumber)
{
    for(int i = 0; i < bufWidth; i++)
    {
        int curr = i+lineNumber*bufWidth;
        buffer[curr] = 0;
        color[curr] = 7; // clear the buffer to white
        
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

menuItem getItem(char* name, int val, char** label, int min, int max, void (*functionPointer)(int, settings*))
{
    menuItem item;
    item.name = name;
    item.val = val;
    item.label = label;
    item.min = min;
    item.max = max;
    item.functionPointer = functionPointer;
    return item;
}

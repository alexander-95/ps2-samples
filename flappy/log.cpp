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

void Log::drawChar(GSGLOBAL* gsGlobal, GSTEXTURE* font, char ascii, u8 x, u8 y, u8 style, u8 color)
{
    ascii -= 32;
    u8 charWidth = 7, charHeight = 9;
    u8 ascii_x = ascii % 18, ascii_y = ascii / 18;
    int offset = 0;
    if(style == 1)offset = 63;

    u64 BLACK = GS_SETREG_RGBAQ(0x00,0x00,0x00,0x80,0x00);
    u64 RED = GS_SETREG_RGBAQ(0x80,0x00,0x00,0x80,0x00);
    u64 GREEN = GS_SETREG_RGBAQ(0x00,0x80,0x00,0x80,0x00);
    u64 BLUE = GS_SETREG_RGBAQ(0x00,0x00,0x80,0x80,0x00);
    u64 CYAN = GS_SETREG_RGBAQ(0x00,0x80,0x80,0x80,0x00);
    u64 MAGENTA = GS_SETREG_RGBAQ(0x80,0x00,0x80,0x80,0x00);
    u64 YELLOW = GS_SETREG_RGBAQ(0x80,0x80,0x00,0x80,0x00);
    u64 WHITE = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);

    u64 COLOR[] = {BLACK, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE};
    
    gsKit_prim_quad_texture(gsGlobal, font,
                            x*charWidth, y*charHeight,     // x1, y1
                            ascii_x*charWidth, ascii_y*charHeight+offset,     // u1, v1

                            x*charWidth, (y+1)*charHeight,   // x2, y2
                            ascii_x*charWidth, (ascii_y+1)*charHeight+offset,   // u2, v2

                            (x+1)*charWidth, y*charHeight,   // x3, y3
                            (ascii_x+1)*charWidth, ascii_y*charHeight+offset,   // u3, v3

                            (x+1)*charWidth, (y+1)*charHeight, // x4, y4
                            (ascii_x+1)* charWidth, (ascii_y+1)*charHeight+offset, // u4, v4
                            6,COLOR[color]);
}


void Log::drawBuffer(GSGLOBAL* gsGlobal, GSTEXTURE* font, u8 style)
{
    int i, j;
    enum mode{WRAP_AROUND, SCROLLING};
    u8 scrollingMode = wrap;
    if(scrollingMode == WRAP_AROUND)
    {
        for(i = 0; i < bufHeight; i++)
        {
            for(j = 0; j < bufWidth; j++) drawChar(gsGlobal, font, buffer[(i*bufWidth)+j], j, i, style,color[(i*bufWidth)+j]);
        }
    }
    else if(scrollingMode == SCROLLING)
    {
        for(i = index; i < index+bufHeight; i++)
        {
            for(j = 0; j < bufWidth; j++) drawChar(gsGlobal, font, buffer[((i%bufHeight)*bufWidth)+j], j, (i-index), style, color[((i%bufHeight)*bufWidth)+j]);
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

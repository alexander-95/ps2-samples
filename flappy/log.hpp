#ifndef LOG_HPP_
#define LOG_HPP_

#include <gsKit.h>
#include <stdio.h>

// forward declaration of bird class
class Bird;

struct log
{
    int index;
    char* buffer;
    char* logfile;
    u8 logToFile;
    u8 logToScreen;
    u8 bufWidth;
    u8 bufHeight;
    u8 wrap; // bool to indicate whether we've filled up the screen
};

struct menuItem
{
    int val;
    char* name;
    char** label;
    int min, max;
    void (*functionPointer)(int, struct settings*);
};

struct settings
{
    GSGLOBAL* gsGlobal;
    Bird* bird;
    int* time;
    int* score;
    int* highScore;
    u8* font;
};

void logMessage(GSGLOBAL* gsGlobal, GSTEXTURE* font, struct log* l, char* msg);
void setLine(struct log* l, int lineNumber, char* str);
void clearLine(struct log* l, int lineNumber);
void clearBuffer(struct log* l);

#endif

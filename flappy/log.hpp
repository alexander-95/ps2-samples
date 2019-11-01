#ifndef LOG_HPP
#define LOG_HPP

#include <gsKit.h>
#include <stdio.h>

// forward declaration of bird class
class Bird;

class Log
{
public:
    s8 index;
    char* buffer;
    char* logfile;
    u8 logToFile;
    u8 logToScreen;
    u8 bufWidth;
    u8 bufHeight;
    u8 wrap; // bool to indicate whether we've filled up the screen

    Log();
    ~Log();
    void logMessage(char* msg);
    void setLine(int lineNumber, char* msg);
    void clearLine(int lineNumber);
    void clearBuffer();
};

typedef struct menuItem
{
    int val;
    char* name;
    char** label;
    int min, max;
    void (*functionPointer)(int, struct settings*);
}menuItem;

typedef struct settings
{
    GSGLOBAL* gsGlobal;
    Bird* bird;
    int* time;
    int* score;
    int* highScore;
    u8* font;
}settings;

#endif

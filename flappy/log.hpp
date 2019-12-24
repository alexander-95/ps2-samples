#ifndef LOG_HPP
#define LOG_HPP

#include <gsKit.h>
#include <stdio.h>

// forward declaration of bird class
class Bird;

class Log
{
private:
    void clearLine(int lineNumber);
    void clearBuffer();
    void logToFile(char* msg);
    void init();
    
public:
    s8 index;
    char* buffer;
    u8* color;
    char* logfile;
    u8 logToScreen;
    u8 bufWidth;
    u8 bufHeight;
    u8 wrap; // bool to indicate whether we've filled up the screen

    Log();
    Log(char* logfile);
    ~Log();
    void logMessage(char* msg);
    void debug(char* msg, ...);
    
};

typedef struct settings
{
    GSGLOBAL* gsGlobal;
    Bird* bird;
    int* time;
    int* score;
    int* highScore;
    u8* font;
}settings;

typedef struct menuItem
{
    int val;
    char* name;
    char** label;
    int min, max;
    void (*functionPointer)(int, settings*);
}menuItem;

menuItem getItem(char* name, int val, char** label, int min, int max, void (*functionPointer)(int, settings*));

#endif

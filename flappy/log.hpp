#ifndef LOG_HPP_
#define LOG_HPP_

#include <gsKit.h>
#include <stdio.h>

// forward declaration of bird class
class Bird;

struct log
{
    int index;
    char buffer[56*90];
    char* logfile;
    u8 logToFile;
    u8 logToScreen;
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
    Bird* bird;
    int* time;
    int* score;
    int* highScore;
};

void logMessage(GSGLOBAL* gsGlobal, GSTEXTURE* font, struct log* l, char* msg);
void setLine(char* buffer, int lineNumber, char* str);
void clearLine(char* buffer, int lineNumber);
void clearBuffer(struct log* l);
void drawMenu(struct log* l, int x, int y, int w, int h, char* title, int cursor, struct menuItem* item);
void clearMenu(struct log* l, int x, int y, int w, int h);
void setCursor(struct log* l, int x, int y, int w, int h, int cursor);
void refreshLabel(struct log* l, int x, int y, int w, int h, int cursor, struct menuItem* item);

#endif

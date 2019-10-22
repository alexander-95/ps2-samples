#ifndef LOG_HPP_
#define LOG_HPP_

#include <gsKit.h>
#include <stdio.h>

struct log
{
    int index;
    char buffer[56*90];
    char* logfile;
    u8 logToFile;
    u8 logToScreen;
    u8 wrap; // bool to indicate whether we've filled up the screen
};

void logMessage(GSGLOBAL* gsGlobal, GSTEXTURE* font, struct log* l, char* msg);
void setLine(char* buffer, int lineNumber, char* str);
void clearLine(char* buffer, int lineNumber);
void clearBuffer(struct log* l);
void drawMenu(struct log* l, int x, int y, int w, int h, char* title, int cursor);
void clearMenu(struct log* l, int x, int y, int w, int h);
void setCursor(struct log* l, int x, int y, int w, int h, int cursor);

#endif

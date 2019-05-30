#ifndef LOG_H_
#define LOG_H_

#include <gsKit.h>
#include <stdio.h>

struct log
{
    int index;
    char buffer[28*45];
    char* logfile;
    u8 logToFile;
    u8 logToScreen;
};

void logMessage(GSGLOBAL* gsGlobal, GSTEXTURE* font, struct log* l, char* msg);
void setLine(char* buffer, int lineNumber, char* str);

#endif

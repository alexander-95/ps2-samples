/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# libmc API sample.
*/

#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <fileio.h>
#include <malloc.h>
#include <libmc.h>
#include <stdio.h>
#include <string.h>
#include "icon.h"


//
// TODO: Update comments to reflect recent modifications to libmc (ie: support for
//       MCMAN/MCSERV or XMCMAN/XMCSERV, asynchronous support etc.
//


//#define TYPE_MC
#define TYPE_XMC

void LoadModules(void);
int CreateSave(void);

#define ARRAY_ENTRIES	64
static mcTable mcDir[ARRAY_ENTRIES] __attribute__((aligned(64)));
static int mc_Type, mc_Free, mc_Format;

int main() {
    int fd, ret;
    int i;

    // Initialise
    SifInitRpc(0);
    LoadModules();
    
#ifdef TYPE_MC
    if(mcInit(MC_TYPE_MC) < 0) {
        printf("Failed to initialise memcard server!\n");
        SleepThread();
    }
#else
    if(mcInit(MC_TYPE_XMC) < 0) {
        printf("Failed to initialise memcard server!\n");
        SleepThread();
    }
#endif
        
    printf("\nMemory card library example code - by Sjeep\n\n");

    // int mcGetInfo(int port, int slot, int* type, int* free, int* format);
    //
    // mcGetInfo retrieves memcard state info, such as the memcard type, free blocks and
    // the format status.
    //
    // mcGetInfo is passed pointers to three variables, which are filled upon completion
    // of the getinfo rpc command. The mcGetInfo return values are as follows:
    //
    //  0 : The same memory card has been connected since the last mcGetInfo call.
    // -1 : Switched to a formatted memory card.
    // -2 : Switched to an unformatted memory card.
    // -10 or less : The memory card could not be detected.
    //
    // NOTE: With the MCMAN/MCSERV, *format is always returned as 0 regardless of if
    //       if the memcard is formatted or not.
    
    // Since this is the first call, -1 should be returned.
    mcGetInfo(0, 0, &mc_Type, &mc_Free, &mc_Format);
    mcSync(0, NULL, &ret);
    printf("mcGetInfo returned %d\n",ret);
    printf("Type: %d Free: %d Format: %d\n\n", mc_Type, mc_Free, mc_Format);

    // Assuming that the same memory card is connected, this should return 0
    mcGetInfo(0,0,&mc_Type,&mc_Free,&mc_Format);
    mcSync(0, NULL, &ret);
    printf("mcGetInfo returned %d\n",ret);
    printf("Type: %d Free: %d Format: %d\n\n", mc_Type, mc_Free, mc_Format);
    
    // int mcGetDir(int port, int slot, char *name, unsigned mode, int maxent, mcTable* table);
    //
    // mcGetDir retrieves the directory structure of a specific path on the memory card.
    //
    // The filename is relative to the root of the memory card. Wildcards such as '*' and '?'
    // may be used. "maxent" is the maximum number of mcTable elements your array specified
    // by "table" can hold. The mc_getdir return values are as follows:
    //
    // 0 or more : The number of file entries that were obtained.
    // -2 : The memory card is unformatted
    // -4 : A non-existant path was specified in the "name" parameter
    // -10 or less : The memory card could not be detected.

    mcGetDir(0, 0, "/*", 0, ARRAY_ENTRIES - 10, mcDir);
    mcSync(0, NULL, &ret);
    printf("mcGetDir returned %d\n\nListing of root directory on memory card:\n\n", ret);
    
    for(i=0; i < ret; i++)
    {
        if(mcDir[i].attrFile & MC_ATTR_SUBDIR)
        {
            printf("[DIR] %s\n", mcDir[i].name);
        }
        else
        {
            printf("%s - %d bytes\n", mcDir[i].name, mcDir[i].fileSizeByte);
        }
    }

    // Check if existing save is present
    fd = fioOpen("mc0:PS2DEV/icon.sys", O_RDONLY);
    if(fd <= 0) {
        
        printf("\nNo previous save exists, creating...\n");
        if((ret = CreateSave()) < 0) {
            printf("Failed to create save! Errorno: %d\n",ret);
            SleepThread();
        }

    } else {
        printf("\nPrevious save exists, listing directory\n\n");
        ret = mcGetDir(0, 0, "/PS2DEV/*", 0, ARRAY_ENTRIES, mcDir);
        printf("mcGetDir returned %d\n\n", ret);
        for(i=0; i < ret; i++)
        {
            if(mcDir[i].attrFile & MC_ATTR_SUBDIR)
            {
                printf("[DIR] %s\n", mcDir[i].name);
            }
            else
            {
                printf("%s - %d bytes\n", mcDir[i].name, mcDir[i].fileSizeByte);
            }
        }
    }

    // Return to the browser, so you can see the PS2Dev icon :)
    SifExitRpc();
    return 0;
}

int CreateSave(void)
{    
    int mc_fd, icon_fd;
    
    if(fioMkdir("mc0:PS2DEV") < 0) return -1;

    // Write icon.sys to the memory card (Note that this filename is fixed)
    mc_fd = fioOpen("mc0:PS2DEV/icon.sys",O_WRONLY | O_CREAT);
    if(mc_fd < 0) return -2;

    fioWrite(mc_fd, &sys_array, sys_size);
    fioClose(mc_fd);
    printf("icon.sys written sucessfully.\n");

    icon_fd = fioOpen("mc0:PS2DEV/icon.icn",O_WRONLY | O_CREAT);
    if(icon_fd < 0) return -6;

    fioWrite(icon_fd,icn_array, icn_size);
    fioClose(icon_fd);
    printf("icon.icn written sucessfully.\n");

    return 0;
}


void LoadModules(void)
{
    int ret;

#ifdef TYPE_MC
    ret = SifLoadModule("rom:SIO2MAN", 0, NULL);
    if (ret < 0) {
        printf("Failed to load module: SIO2MAN");
        SleepThread();
    }

    ret = SifLoadModule("rom:MCMAN", 0, NULL);
    if (ret < 0) {
        printf("Failed to load module: MCMAN");
        SleepThread();
    }

    ret = SifLoadModule("rom:MCSERV", 0, NULL);
    if (ret < 0) {
        printf("Failed to load module: MCSERV");
        SleepThread();
    }
#else
    ret = SifLoadModule("rom:XSIO2MAN", 0, NULL);
    if (ret < 0) {
        printf("Failed to load module: SIO2MAN");
        SleepThread();
    }

    ret = SifLoadModule("rom:XMCMAN", 0, NULL);
    if (ret < 0) {
        printf("Failed to load module: MCMAN");
        SleepThread();
    }

    ret = SifLoadModule("rom:XMCSERV", 0, NULL);
    if (ret < 0) {
        printf("Failed to load module: MCSERV");
        SleepThread();
    }
#endif

}
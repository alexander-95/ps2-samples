#  ____     ___ ____
#  ____|   |    ____|
# |     ___|   |____

EE_BIN = flappy.elf
EE_OBJS = flappy.o bg.o spritesheet.o controller.o log.o graphics.o draw.o

EE_INCLUDES    = /usr/local/ps2dev/ps2sdk/ee/include
GSKIT_INCLUDES = /usr/local/ps2dev/gsKit/include
SDK_COMMON     = /usr/local/ps2dev/ps2sdk/common/include
GSKIT_LIB      = /usr/local/ps2dev/gsKit/lib
EE_LIB         = /usr/local/ps2dev/ps2sdk/ee/lib
CRT            = /usr/local/ps2dev/ps2sdk/ee/startup/crt0.o
LINKFILE       = /usr/local/ps2dev/ps2sdk/ee/startup/linkfile
IOP            = /usr/local/ps2dev/ps2sdk/iop/include

all:
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c controller.c -o controller.o
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c log.c -o log.o
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c graphics.c -o graphics.o
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c draw.c -o draw.o

	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c bg.c -o bg.o
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c spritesheet.c -o spritesheet.o
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c font.c -o font.o

	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES)  -c flappy.c -o flappy.o
	ee-gcc -T$(LINKFILE) -mno-crt0  -L$(GSKIT_LIB) -L$(EE_LIB) -o flappy.elf $(CRT) flappy.o controller.o log.o graphics.o draw.o bg.o spritesheet.o font.o -Xlinker --start-group  -laudsrv -lgskit_toolkit -lgskit -ldmakit -lpad -lc -lkernel -lm -lfileXio -ldebug -Xlinker --end-group

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

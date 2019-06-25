#  ____     ___ ____
#  ____|   |    ____|
# |     ___|   |____

BIN        = flappy.elf
OBJS       = flappy.o controller.o log.o graphics.o draw.o ioman.o
TEX_OBJS   = bg.o spritesheet.o font.o
AUDIO_OBJS = sfx_point.o sfx_wing.o sfx_hit.o sfx_die.o sfx_swooshing.o
IRX_OBJS   = libsd.o audsrv.o

EE_INCLUDES    = /usr/lib/ps2dev/ps2sdk/ee/include
GSKIT_INCLUDES = /usr/lib/ps2dev/gsKit/include
SDK_COMMON     = /usr/lib/ps2dev/ps2sdk/common/include
GSKIT_LIB      = /usr/lib/ps2dev/gsKit/lib
EE_LIB         = /usr/lib/ps2dev/ps2sdk/ee/lib
CRT            = /usr/lib/ps2dev/ps2sdk/ee/startup/crt0.o
LINKFILE       = /usr/lib/ps2dev/ps2sdk/ee/startup/linkfile
IOP            = /usr/lib/ps2dev/ps2sdk/iop/include

all:
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c controller.c -o controller.o
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c log.c -o log.o
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c graphics.c -o graphics.o
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c draw.c -o draw.o
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -c ioman.c -o ioman.o

	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -c bg.c -o bg.o
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -c spritesheet.c -o spritesheet.o
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -c font.c -o font.o

	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -c audio/sfx_point.c -o sfx_point.o
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -c audio/sfx_wing.c -o sfx_wing.o
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -c audio/sfx_hit.c -o sfx_hit.o
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -c audio/sfx_die.c -o sfx_die.o
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -c audio/sfx_swooshing.c -o sfx_swooshing.o

	$(PS2SDK)/bin/bin2s $(PS2SDK)/iop/irx/libsd.irx libsd.s libsd_irx
	ee-as -G0 libsd.s -o libsd.o
	$(PS2SDK)/bin/bin2s $(PS2SDK)/iop/irx/audsrv.irx audsrv.s audsrv_irx
	ee-as -G0 audsrv.s -o audsrv.o

	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -I$(IOP) -I. -c flappy.c -o flappy.o
	ee-gcc -T$(LINKFILE) -mno-crt0  -L$(GSKIT_LIB) -L$(EE_LIB) -o flappy.elf $(CRT) $(OBJS) $(TEX_OBJS) $(AUDIO_OBJS) $(IRX_OBJS) -Xlinker --start-group -laudsrv -lgskit_toolkit -lgskit -ldmakit -lpad -lc -lkernel -lm -ldebug -lpatches -Xlinker --end-group

clean:
	rm -f $(BIN) $(OBJS) $(TEX_OBJS) $(AUDIO_OBJS) $(IRX_OBJS)

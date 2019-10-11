#  ____     ___ ____
#  ____|   |    ____|
# |     ___|   |____

BIN        = flappy.elf
OBJS       = flappy.o controller.o log.o graphics.o draw.o ioman.o
TEX_OBJS   = spritesheet.o font.o
AUDIO_OBJS = sfx_point.o sfx_wing.o sfx_hit.o sfx_die.o sfx_swooshing.o
IRX_OBJS   = libsd.o audsrv.o

EE_INCLUDES    = $(PS2SDK)/ee/include
GSKIT_INCLUDES = $(GSKIT)/include
SDK_COMMON     = $(PS2SDK)/common/include
GSKIT_LIB      = $(GSKIT)/lib
EE_LIB         = $(PS2SDK)/ee/lib
CRT            = $(PS2SDK)/ee/startup/crt0.o
LINKFILE       = $(PS2SDK)/ee/startup/linkfile

all:

#	create source files from png images
	python png2abgr32.py -i images/spritesheet.png -o textures/
	python png2abgr32.py -i images/font.png -o textures/

#	compile image sources
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c textures/spritesheet.c -o spritesheet.o
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c textures/font.c -o font.o

#	compile audio sources
	ee-gcc -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c audio/sfx_point.c -o sfx_point.o
	ee-gcc -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c audio/sfx_wing.c -o sfx_wing.o
	ee-gcc -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c audio/sfx_hit.c -o sfx_hit.o
	ee-gcc -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c audio/sfx_die.c -o sfx_die.o
	ee-gcc -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c audio/sfx_swooshing.c -o sfx_swooshing.o

#	compile irx modules
	$(PS2SDK)/bin/bin2s $(PS2SDK)/iop/irx/libsd.irx libsd.s libsd_irx
	ee-as -G0 libsd.s -o libsd.o
	$(PS2SDK)/bin/bin2s $(PS2SDK)/iop/irx/audsrv.irx audsrv.s audsrv_irx
	ee-as -G0 audsrv.s -o audsrv.o

#	create object files
	ee-g++ -D_EE -O2 -G0 -Wall -g -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c controller.cpp -o controller.o
	ee-g++ -D_EE -O2 -G0 -Wall -g -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c log.cpp -o log.o
	ee-g++ -D_EE -O2 -G0 -Wall -g -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c graphics.cpp -o graphics.o
	ee-g++ -D_EE -O2 -G0 -Wall -g -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c draw.cpp -o draw.o
	ee-g++ -D_EE -O2 -G0 -Wall -g -I$(EE_INCLUDES) -I$(SDK_COMMON) -c thirdparty/ioman.cpp -o ioman.o

#	compile game
	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -I. -c flappy.cpp -o flappy.o
	ee-gcc -T$(LINKFILE) -mno-crt0 -L$(GSKIT_LIB) -L$(EE_LIB) -o flappy.elf $(CRT) $(OBJS) $(TEX_OBJS) $(AUDIO_OBJS) $(IRX_OBJS) -Xlinker --start-group -laudsrv -lgskit_toolkit -lgskit -ldmakit -lpad -lc -lkernel -lm -lpatches -lstdc++ -lfileXio --end-group

clean:
	rm -f $(BIN) $(OBJS) $(TEX_OBJS) $(AUDIO_OBJS) $(IRX_OBJS)

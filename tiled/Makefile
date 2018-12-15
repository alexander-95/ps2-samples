#  ____     ___ ____
#  ____|   |    ____|
# |     ___|   |____

EE_BIN =  tiled.elf
EE_OBJS = tiled.o spritesheet.o controller.o map.o map_data.o
TEXTURE_SRC = spritesheet.c spritesheet.h

EE_INCLUDES    = /usr/local/ps2dev/ps2sdk/ee/include
SDK_COMMON     = /usr/local/ps2dev/ps2sdk/common/include
EE_LIB         = /usr/local/ps2dev/ps2sdk/ee/lib
CRT            = /usr/local/ps2dev/ps2sdk/ee/startup/crt0.o
LINKFILE       = /usr/local/ps2dev/ps2sdk/ee/startup/linkfile

GSKIT_INCLUDES = /usr/local/ps2dev/gsKit/include
GSKIT_LIB      = /usr/local/ps2dev/gsKit/lib

all:
	python png2abgr.py spritesheet.png
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c spritesheet.c -o spritesheet.o
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c map_data.c -o map_data.o

	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c controller.cpp -o controller.o
	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c map.cpp -o map.o

	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c tiled.cpp -o tiled.o
	ee-gcc -T$(LINKFILE) -mno-crt0 -L$(GSKIT_LIB) -L$(EE_LIB) -o $(EE_BIN) $(CRT) $(EE_OBJS) -Xlinker --start-group  -laudsrv -lgskit_toolkit -lgskit -ldmakit -lpad -lc -lkernel -lfileXio -lstdc++ --end-group

	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c test.cpp -o test.o
	ee-gcc -T$(LINKFILE) -mno-crt0 -L$(GSKIT_LIB) -L$(EE_LIB) -o test.elf $(CRT) test.o spritesheet.o -Xlinker --start-group  -laudsrv -lgskit_toolkit -lgskit -ldmakit -lpad -lc -lkernel -lfileXio -lstdc++ --end-group

#ee-gcc -T$(LINKFILE) -mno-crt0 -L$(EE_LIB) -o $(EE_BIN) $(CRT) $(EE_OBJS) -Xlinker --start-group -lc -lkernel -lstdc++ --end-group

clean:
	rm -f $(EE_BIN) $(EE_OBJS) $(TEXTURE_SRC)

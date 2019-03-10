#  ____     ___ ____
#  ____|   |    ____|
# |     ___|   |____

EE_BIN =  tiled.elf
EE_OBJS = tiled.o spritesheet.o controller.o map.o map_data.o character.o mario.o block.o goomba.o koopa.o pickup.o pickups.o hud.o HUD.o title.o cursor.o
TEXTURE_SRC = spritesheet.c spritesheet.h mario.c mario.h goomba.c goomba.h koopa.c koopa.h pickups.c pickups.h hud.c hud.h title.c title.h cursor.c cursor.o

EE_INCLUDES    = /usr/local/ps2dev/ps2sdk/ee/include
SDK_COMMON     = /usr/local/ps2dev/ps2sdk/common/include
EE_LIB         = /usr/local/ps2dev/ps2sdk/ee/lib
CRT            = /usr/local/ps2dev/ps2sdk/ee/startup/crt0.o
LINKFILE       = /usr/local/ps2dev/ps2sdk/ee/startup/linkfile

GSKIT_INCLUDES = /usr/local/ps2dev/gsKit/include
GSKIT_LIB      = /usr/local/ps2dev/gsKit/lib

all:
	python png2abgr.py spritesheet.png
	python png2abgr.py mario.png
	python png2abgr.py goomba.png
	python png2abgr.py koopa.png
	python png2abgr.py pickups.png
	python png2abgr.py hud.png
	python png2abgr.py title.png
	python png2abgr.py cursor.png

	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c spritesheet.c -o spritesheet.o
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c map_data.c -o map_data.o
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c mario.c -o mario.o
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c goomba.c -o goomba.o
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c koopa.c -o koopa.o
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c pickups.c -o pickups.o
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c hud.c -o hud.o
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c title.c -o title.o
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c cursor.c -o cursor.o

	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c controller.cpp -o controller.o
	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c map.cpp -o map.o
	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c character.cpp -o character.o
	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c block.cpp -o block.o
	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c pickup.cpp -o pickup.o
	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c HUD.cpp -o HUD.o

	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c tiled.cpp -o tiled.o
	ee-gcc -T$(LINKFILE) -mno-crt0 -L$(GSKIT_LIB) -L$(EE_LIB) -o $(EE_BIN) $(CRT) $(EE_OBJS) -Xlinker --start-group  -laudsrv -lgskit_toolkit -lgskit -ldmakit -lpad -lc -lkernel -lfileXio -lstdc++ --end-group

clean:
	rm -f $(EE_BIN) $(EE_OBJS) $(TEXTURE_SRC)

#  ____     ___ ____
#  ____|   |    ____|
# |     ___|   |____

EE_BIN =  tiled.elf
EE_OBJS = tiled.o spritesheet.o controller.o map.o map_data.o character.o mario.o block.o goomba.o koopa.o pickup.o pickups.o hud.o HUD.o title.o cursor.o levelBuilder.o graphics.o draw.o
TEXTURE_SRC = spritesheet.c spritesheet.h mario.c mario.h goomba.c goomba.h koopa.c koopa.h pickups.c pickups.h hud.c hud.h title.c title.h cursor.c cursor.o

EE_INCLUDES    = $(PS2SDK)/ee/include
SDK_COMMON     = $(PS2SDK)/common/include
EE_LIB         = $(PS2SDK)/ee/lib
CRT            = $(PS2SDK)/ee/startup/crt0.o
LINKFILE       = $(PS2SDK)/ee/startup/linkfile

GSKIT_INCLUDES = $(GSKIT)/include
GSKIT_LIB      = $(GSKIT)/lib

all:
	python png2abgr32.py -i images/spritesheet.png -o textures/
	python png2abgr32.py -i images/mario.png -o textures/
	python png2abgr32.py -i images/goomba.png -o textures/
	python png2abgr32.py -i images/koopa.png -o textures/
	python png2abgr32.py -i images/pickups.png -o textures/
	python png2abgr32.py -i images/hud.png -o textures/
	python png2abgr32.py -i images/title.png -o textures/
	python png2abgr32.py -i images/cursor.png -o textures/

	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c textures/spritesheet.c -o spritesheet.o
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c map_data.c -o map_data.o
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c textures/mario.c -o mario.o
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c textures/goomba.c -o goomba.o
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c textures/koopa.c -o koopa.o
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c textures/pickups.c -o pickups.o
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c textures/hud.c -o hud.o
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c textures/title.c -o title.o
	ee-gcc -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c textures/cursor.c -o cursor.o

	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c controller.cpp -o controller.o
	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c graphics.cpp -o graphics.o
	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c draw.cpp -o draw.o
	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c levelBuilder.cpp -o levelBuilder.o
	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c map.cpp -o map.o
	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c character.cpp -o character.o
	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c block.cpp -o block.o
	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c pickup.cpp -o pickup.o
	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c HUD.cpp -o HUD.o

	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c tiled.cpp -o tiled.o
	ee-gcc -T$(LINKFILE) -mno-crt0 -L$(GSKIT_LIB) -L$(EE_LIB) -o $(EE_BIN) $(CRT) $(EE_OBJS) -Xlinker --start-group  -laudsrv -lgskit_toolkit -lgskit -ldmakit -lpad -lc -lkernel -lfileXio -lstdc++ --end-group

clean:
	rm -f $(EE_BIN) $(EE_OBJS) $(TEXTURE_SRC)

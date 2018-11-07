#  ____     ___ ____
#  ____|   |    ____|
# |     ___|   |____

EE_BIN = flappy.elf
EE_OBJS = flappy.o

EE_INCLUDES    = /usr/local/ps2dev/ps2sdk/ee/include
GSKIT_INCLUDES = /usr/local/ps2dev/gsKit/include
SDK_COMMON     = /usr/local/ps2dev/ps2sdk/common/include
GSKIT_LIB      = /usr/local/ps2dev/gsKit/lib
EE_LIB         = /usr/local/ps2dev/ps2sdk/ee/lib
CRT            = /usr/local/ps2dev/ps2sdk/ee/startup/crt0.o
LINKFILE       = /usr/local/ps2dev/ps2sdk/ee/startup/linkfile
IOP            = /usr/local/ps2dev/ps2sdk/iop/include

LIBPNG_INCLUDES = /usr/local/ps2dev/ps2sdk-ports/libpng/include
LIBPNG          = /usr/local/ps2dev/ps2sdk-ports/libpng

LIBTIFF_INCLUDES = /usr/local/ps2dev/ps2sdk-ports/libtiff/include
LIBTIFF          = /usr/local/ps2dev/ps2sdk-ports/libtiff

LIBJPEG_INCLUDES = /usr/local/ps2dev/ps2sdk-ports/libjpeg/include
LIBJPEG          = /usr/local/ps2dev/ps2sdk-ports/libjpeg/lib

ZLIB_INCLUDES    = /usr/local/ps2dev/ps2sdk-ports/zlib
ZLIB             = /usr/local/ps2dev/ps2sdk-ports/zlib

all:
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c controller.c -o controller.o
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c bg.c -o bg.o
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c spritesheet.c -o spritesheet.o
	ee-gcc -D_EE -O2 -G0 -Wall -DHAVE_LIBJPEG -DHAVE_LIBTIFF -DHAVE_LIBPNG -DHAVE_ZLIB -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -I$(LIBPNG_INCLUDES) -I$(LIBTIFF_INCLUDES) -I$(LIBJPEG_INCLUDES) -I$(ZLIB_INCLUDES) -c flappy.c -o flappy.o
	ee-gcc -T$(LINKFILE) -mno-crt0 -L$(LIBJPEG) -L$(LIBTIFF) -L$(ZLIB) -L$(LIBPNG) -L$(GSKIT_LIB) -L$(EE_LIB) -o flappy.elf $(CRT) flappy.o controller.o bg.o spritesheet.o -Xlinker --start-group  -laudsrv -lgskit_toolkit -lgskit -ldmakit -lpad -lc -lkernel -lpng -ljpeg -ltiff -lz -lm -lfileXio -ldebug -Xlinker --end-group

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

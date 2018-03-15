#  ____     ___ ____
#  ____|   |    ____|
# |     ___|   |____

EE_BIN =  snake.elf
EE_OBJS = snake.o

EE_INCLUDES    = /usr/local/ps2dev/ps2sdk/ee/include
GSKIT_INCLUDES = /usr/local/ps2dev/gsKit/include
SDK_COMMON     = /usr/local/ps2dev/ps2sdk/common/include
GSKIT_LIB      = /usr/local/ps2dev/gsKit/lib
EE_LIB         = /usr/local/ps2dev/ps2sdk/ee/lib
CRT            = /usr/local/ps2dev/ps2sdk/ee/startup/crt0.o
LINKFILE       = /usr/local/ps2dev/ps2sdk/ee/startup/linkfile

all:
	ee-gcc -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -I$(GSKIT_INCLUDES) -c snake.c -o snake.o
	ee-gcc -T$(LINKFILE) -mno-crt0 -L$(GSKIT_LIB) -L$(EE_LIB) -o snake.elf $(CRT) snake.o -Xlinker --start-group  -lgskit_toolkit -lgskit -ldmakit -lpad -lc -lkernel --end-group

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

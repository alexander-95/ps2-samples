#  ____     ___ ____
#  ____|   |    ____|
# |     ___|   |____

EE_BIN =  hello.elf
EE_OBJS = hello.o

EE_INCLUDES    = /usr/local/ps2dev/ps2sdk/ee/include
GSKIT_INCLUDES = /usr/local/ps2dev/gsKit/include
SDK_COMMON     = /usr/local/ps2dev/ps2sdk/common/include
GSKIT_LIB      = /usr/local/ps2dev/gsKit/lib
EE_LIB         = /usr/local/ps2dev/ps2sdk/ee/lib
CRT            = /usr/local/ps2dev/ps2sdk/ee/startup/crt0.o
LINKFILE       = /usr/local/ps2dev/ps2sdk/ee/startup/linkfile
IOP            = /usr/local/ps2dev/ps2sdk/iop/include

all:
	ee-g++ -D_EE -O2 -G0 -Wall -g -fno-builtin-printf -I$(EE_INCLUDES) -I$(SDK_COMMON) -c hello.cpp -o hello.o
	ee-gcc -T$(LINKFILE) -mno-crt0 -L$(EE_LIB) -o hello.elf $(CRT) hello.o -Xlinker --start-group -lpad -lc -lkernel -lm -lfileXio -ldebug -Xlinker --end-group

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

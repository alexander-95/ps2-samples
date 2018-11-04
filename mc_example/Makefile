#  ____     ___ ____
#  ____|   |    ____|
# |     ___|   |____

EE_BIN = mc_example.elf
EE_OBJS = mc_example.o

EE_INCLUDES    = /usr/local/ps2dev/ps2sdk/ee/include
GSKIT_INCLUDES = /usr/local/ps2dev/gsKit/include
SDK_COMMON     = /usr/local/ps2dev/ps2sdk/common/include
GSKIT_LIB      = /usr/local/ps2dev/gsKit/lib
EE_LIB         = /usr/local/ps2dev/ps2sdk/ee/lib
CRT            = /usr/local/ps2dev/ps2sdk/ee/startup/crt0.o
LINKFILE       = /usr/local/ps2dev/ps2sdk/ee/startup/linkfile
IOP            = /usr/local/ps2dev/ps2sdk/iop/include

all:
	ee-gcc -D_EE -O2 -G0 -Wall  -I$(EE_INCLUDES) -I$(SDK_COMMON) -I.  -c icon.c -o icon.o
	ee-gcc -D_EE -O2 -G0 -Wall  -I$(EE_INCLUDES) -I$(SDK_COMMON) -I.  -c mc_example.c -o mc_example.o
	ee-gcc -mno-crt0 -T$(LINKFILE) -D_EE -O2 -G0 -Wall -o mc_example.elf $(CRT)   mc_example.o icon.o  -L$(EE_LIB)  -lmc -lc -lc -lkernel

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

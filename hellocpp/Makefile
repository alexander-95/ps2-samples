#  ____     ___ ____
#  ____|   |    ____|
# |     ___|   |____

EE_BIN =  hello.elf
EE_OBJS = hello.o main.o

EE_INCLUDES    = /usr/local/ps2dev/ps2sdk/ee/include
SDK_COMMON     = /usr/local/ps2dev/ps2sdk/common/include
EE_LIB         = /usr/local/ps2dev/ps2sdk/ee/lib
CRT            = /usr/local/ps2dev/ps2sdk/ee/startup/crt0.o
LINKFILE       = /usr/local/ps2dev/ps2sdk/ee/startup/linkfile

all:
	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c hello.cpp -o hello.o
	ee-g++ -D_EE -O2 -G0 -Wall -I$(EE_INCLUDES) -I$(SDK_COMMON) -c main.cpp -o main.o
	ee-gcc -T$(LINKFILE) -mno-crt0 -L$(EE_LIB) -o $(EE_BIN) $(CRT) $(EE_OBJS) -Xlinker --start-group -lc -lkernel -lstdc++ --end-group

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

#  ____     ___ ____
#  ____|   |    ____|
# |     ___|   |____

EE_BIN = regDump.elf
EE_OBJS = regDump.o

EE_LIB   = /usr/lib/ps2dev/ps2sdk/ee/lib
CRT      = /usr/lib/ps2dev/ps2sdk/ee/startup/crt0.o
LINKFILE = /usr/lib/ps2dev/ps2sdk/ee/startup/linkfile

all:
	ee-gcc -c regDump.S -o regDump.o
	ee-gcc -mno-crt0 -T$(LINKFILE) -D_EE -G0 -Wall -o regDump.elf $(CRT) regDump.o -L$(EE_LIB) -lc -lkernel
clean:
	rm -f $(EE_BIN) $(EE_OBJS)

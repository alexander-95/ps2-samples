#  ____     ___ ____
#  ____|   |    ____|
# |     ___|   |____

EE_BIN = regDump.elf
EE_OBJS = regDump.o

all: $(EE_BIN)
	$(EE_STRIP) --strip-all $(EE_BIN)

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

run: $(EE_BIN)
	ps2client execee host:$(EE_BIN)

reset:
	ps2client reset

compile:
	ee-gcc -S regDump.c -o regDump.S

assemble:
	ee-gcc -c regDump.S -o regDump.o
	ee-gcc -mno-crt0 -T/usr/lib/ps2dev/ps2sdk/ee/startup/linkfile -D_EE -G0 -Wall -o regDump.elf /usr/lib/ps2dev/ps2sdk/ee/startup/crt0.o regDump.o -L/usr/lib/ps2dev/ps2sdk/ee/lib -lc -lkernel

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal

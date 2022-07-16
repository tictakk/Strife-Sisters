all: sisterstrife.pce

include ../Make_ex.inc

# SRC = promotion.c menu.c overworld.c battle.c
SRC = sisterstrife.c
CFLAGS ?= -v -fno-recursive -msmall

sisterstrife.pce: $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LIBS)

# menu.o: $(CC) $(CFLAGS) menu.c $(LIBS)
#
# overworld.o: $(CC) $(CFLAGS) overworld.c $(LIBS)


clean:
	rm -f *.pce *.lst *.sym *.s

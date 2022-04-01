all: promotion.pce

include ../Make_ex.inc

# SRC = promotion.c menu.c overworld.c battle.c
SRC = promotion.c
CFLAGS ?= -v -fno-recursive -msmall

promotion.pce: $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LIBS)

# menu.o: $(CC) $(CFLAGS) menu.c $(LIBS)
#
# overworld.o: $(CC) $(CFLAGS) overworld.c $(LIBS)


clean:
	rm -f *.pce *.lst *.sym *.s

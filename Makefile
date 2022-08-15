# all: sisterstrife.pce
#
# SRC = sisterstrife.c
# CFLAGS ?= -v -fno-recursive #-msmall
#
# sisterstrife.pce: $(SRC)
# 	$(CC) $(CFLAGS) $(SRC) $(LIBS)
#
# clean:
# 	rm -f *.pce *.lst *.sym *.s

all: strifesisters.pce

include ../Make_ex.inc

CFLAGS = -msmall -fno-recursive -v
LIBS =

strifesisters.pce: sisterstrife.c
	$(CC) -fsigned-char $(CFLAGS) $(LIBS) $<

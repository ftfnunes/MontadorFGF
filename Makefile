IDIR =./include
CC=gcc
CFLAGS=-ansi -Wall -I$(IDIR)

SRC_DIR = ./src

ODIR=obj

_DEPS = DuasPassagens.h macros.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = DuasPassagens.o macros.o Montador.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


Montador: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

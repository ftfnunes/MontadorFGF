IDIR =./include
CC=gcc
CFLAGS=-ansi -Wall -I$(IDIR)

SRC_DIR = ./src

ODIR=obj

_DEPS = DuasPassagens.h macros.h prediretivas.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = DuasPassagens.o macros.o prediretivas.o Montador.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


Montador: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Simulador: obj/simulador.o
	$(CC) -o $@ $(SRC_DIR)/main.c $(ODIR)/simulador.o $(CFLAGS)

obj/simulador.o: $(SRC_DIR)/simulador.c $(IDIR)/simulador.h
	$(CC) -c -o $@ src/simulador.c $(CFLAGS)
	

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

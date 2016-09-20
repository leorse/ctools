IDIR =../
CC=gcc
CFLAGS=-I$(IDIR)
OUTNAME=ctools
EXEDIR=../exe
ODIR=../obj
LDIR =../lib

LIBS=-lm

DEPS = $(IDIR)

OBJ = main.o

main.o: main.c 
	$(CC) -c main.c -o $(ODIR)/main.o $(CFLAGS)

all: $(OBJ)
	gcc -o $(EXEDIR)/$(OUTNAME) $(ODIR)/main.o $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(EXEDIR)/*
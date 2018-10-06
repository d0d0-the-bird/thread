IDIR =.
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=BUILD/obj
LDIR =../lib

LIBS=-lm

_DEPS = thread.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o thread.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	if not exist $(ODIR) (mkdir $(subst /,\,$(ODIR)))
	$(CC) -c -o $@ $< $(CFLAGS)

BUILD/threadtest: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

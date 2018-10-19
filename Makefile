CC=gcc
FLAGS=-std=c11

LIBOUT=libmatrix.a
EXECOUT=matrix

ifdef DEBUG
FLAGS+=-g -O0
endif

INCLUDE=-I . -L .
CFLAGS=$(FLAGS) $(INCLUDE)
LIB=-l matrix

ODIR=obj
SDIR=src

OBJS=matrix.o arithmetic.o inverse.o
_OBJS=$(patsubst %, $(ODIR)/%, $(OBJS))

matrix: lib
	$(CC) $(CFLAGS) matrix.c $(LIB) -o $(EXECOUT)

lib: makeodir $(_OBJS)
	ar rcs $(LIBOUT) $(_OBJS)

makeodir:
	mkdir $(ODIR)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) -c $(FLAGS) -o $@ $<

clean:
	rm -f $(LIBOUT) $(EXECOUT) $(ODIR)/*.o

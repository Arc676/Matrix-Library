CC=gcc
FLAGS=-std=c11

LIBOUT=libmatrix.a
EXECOUT=matrix

ifdef DEBUG
FLAGS+=-g -O0
endif

ifdef THREADSAFE
FLAGS+=-D THREADSAFE
endif

INCLUDE=-I src -L .
CFLAGS=$(FLAGS) $(INCLUDE)
LIB=-l matrix

ODIR=obj
SDIR=src
FDIR=frontend
F2DIR=frontend2

OBJS=matrix.o arithmetic.o inverse.o
_OBJS=$(patsubst %, $(ODIR)/%, $(OBJS))

matrix2: lib
	$(CC) $(CFLAGS) $(F2DIR)/matrix.c $(LIB) -o $(F2DIR)/$(EXECOUT)

matrix: lib
	$(CC) $(CFLAGS) $(FDIR)/matrix.c $(LIB) -o $(FDIR)/$(EXECOUT)

lib: makeodir $(_OBJS)
	ar rcs $(LIBOUT) $(_OBJS)

makeodir:
	mkdir -p $(ODIR)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c $(FLAGS) -o $@ $<

clean:
	rm -f $(LIBOUT) $(EXECOUT) $(ODIR)/*.o

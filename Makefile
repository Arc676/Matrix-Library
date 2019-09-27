CC=gcc
CPP=g++
FLAGS=-std=c11
DEBUGFLAG=

LIBOUT=libmatrix.a
EXECOUT=matrix

ifdef DEBUG
DEBUGFLAG+=-g -O0
endif

ifdef THREADSAFE
FLAGS+=-D THREADSAFE
endif

INCLUDE=-I src -I ExprFix/src
CFLAGS=$(FLAGS) $(INCLUDE) $(DEBUGFLAG)
CPPFLAGS=-std=c++11 $(INCLUDE) $(DEBUGFLAG)
LIB=-L . -l matrix -L ExprFix -l exprfix

ODIR=obj
SDIR=src
FDIR=frontend
F2DIR=frontend2

OBJS=matrix.o arithmetic.o inverse.o
_OBJS=$(patsubst %, $(ODIR)/%, $(OBJS))

matrix2: lib
	$(CPP) -c $(F2DIR)/matrix.cpp $(F2DIR)/memory.cpp $(CPPFLAGS)
	mv *.o $(F2DIR)
	$(CPP) $(F2DIR)/matrix.o $(F2DIR)/memory.o $(LIB) -o $(F2DIR)/$(EXECOUT)

matrix: lib
	$(CC) $(CFLAGS) $(FDIR)/matrix.c $(LIB) -o $(FDIR)/$(EXECOUT)

lib: makeodir $(_OBJS)
	ar rcs $(LIBOUT) $(_OBJS)

makeodir:
	mkdir -p $(ODIR)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c $(FLAGS) -o $@ $<

clean:
	rm -f $(LIBOUT) $(EXECOUT) $(ODIR)/*.o $(F2DIR)/*.o

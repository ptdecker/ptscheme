# Basic Makefile for ptscheme

# Set our paths to traditional locations for files:
#      ./src - c source files
#      ./include - c header files

vpath %.c src
vpath %.h include

CC = clang
CPPFLAGS = -std=c99 -Wall -g -I include
objects = ptscheme.o memmanager.o lispbool.o lispchar.o lispint.o \
          lispstr.o lisperr.o lisppair.o

all: ptscheme

ptscheme: $(objects)
#	clang $(CPPFLAGS) -o ptscheme $(objects)

ptscheme.o: ptscheme.h memmanager.h

memmanager.o: ptscheme.h memmanager.h

lispbool.o: ptscheme.h memmanager.h lispbool.h

lispchar.o: ptscheme.h memmanager.h lispchar.h

lispint.o: ptscheme.h memmanager.h lispint.h

lispstr.o: ptscheme.h memmanager.h lispstr.h

lisperr.o: ptscheme.h memmanager.h lisperr.h

lisppair.o: ptscheme.h memmanager.h lisppair.h

# clean - Implement standard 'clean' target to clean up after ourselves

.PHONY: clean
clean:
	rm -f ptscheme $(objects)
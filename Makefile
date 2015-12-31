# Basic Makefile for ptscheme

# Set our paths to traditional locations for files:
#      ./src - c source files
#      ./include - c header files

vpath %.c src
vpath %.h include

CC = clang
CPPFLAGS = -std=c99 -Wall -g -I include
objects = ptscheme.o memmanager.o lispbool.o lispchar.o lispint.o \
          lispstr.o lisperr.o lisppair.o hashtable.o symbols.o \
          replread.o repleval.o replprint.o environments.o
testedlibs = hashtable.o
tests = hashtabletest.o

all: ptscheme testsuite

ptscheme: $(objects)
#	clang $(CPPFLAGS) -o ptscheme $(objects)

testsuite: $(tests)
	clang $(tests) $(testedlibs) -o testsuite

ptscheme.o:

memmanager.o:

lispbool.o:

lispchar.o:

lispint.o:

lispstr.o:

lisperr.o:

lisppair.o:

hashtable.o:

symbols.o:

replread.o:

repleval.o:

replprint.o:

environments.o:

hashtabletest.o:
	clang $(CPPFLAGS) -c -o hashtabletest.o tests/hashtabletest.c

# test - Implement test target
.PHONY: test
test: ptscheme testsuite
	./testsuite

# clean - Implement standard 'clean' target to clean up after ourselves

.PHONY: clean
clean:
	rm -f ptscheme $(objects)
	rm -f testsuite $(tests)

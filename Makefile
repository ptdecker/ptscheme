# Typical makefile example for OS X that provides the basics

# Set our paths to traditional locations for files:
#      ./src - c source files
#      ./include - c header files
#
# For help on make, refer to: https://www.gnu.org/software/make/manual/make.html

vpath %.c src
vpath %.h include

CC = clang
CPPFLAGS = -std=c99 -Wall -g -I include
objects  = ptscheme.o memmanager.o lispbool.o lispchar.o lispint.o lispstr.o lisperr.o

# Standard targets

all: ptscheme

# Define all our non-standard targets and their dependencies

ptscheme: $(objects)
	clang $(CPPFLAGS) -o ptscheme $(objects)

ptscheme.o: ptscheme.h memmanager.h
memmanager.o: ptscheme.h memmanager.h
lispbool.o: ptscheme.h memmanager.h lispbool.h
lispchar.o: ptscheme.h memmanager.h lispchar.h
lispint.o: ptscheme.h memmanager.h lispint.h
lispstr.o: ptscheme.h memmanager.h lispstr.h
lisperr.o: ptscheme.h memmanager.h lisperr.h

# clean - Implement standard 'clean' target to clean up after ourselves

.PHONY: clean
clean:
	rm -f ptscheme $(objects)

# help - Provide some basic help capabilities by listing available targets

.PHONY: help
help:
	@$(MAKE) -pq | \
	    grep -v -e '^no-such-target' -e '^makefile' -e '^-l' | \
	    awk '/^[^.%][-A-Za-z0-9_]*:/{print substr($$1, 1, length($$1)-1)}' | \
	    sort | \
	    pr --omit-pagination --width=80 --columns=4
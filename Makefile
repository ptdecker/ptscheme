# Typical makefile example for OS X that provides the basics

# Set our paths to traditional locations for files:
#      ./src - c source files
#      ./include - c header files
#
# For help on make, refer to: https://www.gnu.org/software/make/manual/make.html

vpath %.c src
vpath %.h include
CPPFLAGS = -std=c99 -Wall -ggdb -I include

# Standard targets

all: ptscheme clean

# Define all our non-standard targets and their dependencies

ptscheme: ptscheme.o

# clean - Implement standard 'clean' target to clean up after ourselves

.PHONY: clean
clean:
	rm -f *.o

# help - Provide some basic help capabilities by listing available targets

.PHONY: help
help:
	@$(MAKE) -pq | \
	    grep -v -e '^no-such-target' -e '^makefile' -e '^-l' | \
	    awk '/^[^.%][-A-Za-z0-9_]*:/{print substr($$1, 1, length($$1)-1)}' | \
	    sort | \
	    pr --omit-pagination --width=80 --columns=4
ptscheme
========

[![Build Status](https://travis-ci.org/ptdecker/ptscheme.svg?branch=master)](https://travis-ci.org/ptdecker/ptscheme)

### Overview

My take on a Scheme dialect LISP implementation realized in 'C' with enough functionality to
complete the excercises in the Purple Wizard book, "Structure and Interpretation
of Computer Programs"

### Implementation notes and credits

This implementation of Scheme draws heavily from the following sources:

* Daniel Holden's MPC under BSD3
* Peter Michaux's "Scheme from Scratch"

The following changes have been made from Michaux:

* Broke apart into logical (hopefully) modules instead of one big source file
* Inclusion of an error type so that malformed input doesn't kill the REPL
* Handled the singleton boolean object type differently to parallel other objects
* Implemented characters using C-style character literals
* Included support for all C-style escape sequences except for octal and hex
* Echoed the way the singleton boolean was handled for singleton empty
* Handled 'quote', 'define', 'set', 'ok', 'if', and lambda symbols as singletons too
* Implemented hash table-based symbol table [O(1)] as opposed to reusing pairs approach [O(n)]
* Avoided goto-based tail recurrsion call approach

### Getting things running

The following instructions have been tested on OS X v10.10.5.  I'm sure they assume that certain
dev tools are already installed, but I haven't yet tested from a clean OS to confirm the full
set of requirements.  Basically, if you have enough to complie C code with clang (Apple's gcc-style
compliler), can run 'make', and have git installed you're on your way.

Clone:

    $ git clone https://github.com/ptdecker/ptscheme.git
    $ cd ptscheme

Build:

    $ make

Test (not full coverage):

    $ make clean
    $ make
    $ make test

Run:

    $ ./ptscheme

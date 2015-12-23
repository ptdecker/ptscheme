ptscheme
========

My take on a Scheme dialect LISP implementation realized in 'C' with enough functionality to
complete the excercises in the Purple Wizard book, "Structure and Interpretation
of Computer Programs"

This implementation of Scheme draws heavily from the following sources:

* Daniel Holden's MPC under BSD3
* Peter Michaux's "Scheme from Scratch"

The following changes have been made from Michaux:

* Inclusion of an error type so that malformed input doesn't kill the app
* Handled the singleton boolean object type differently to parallel other objects
* Implemented characters using C-style character literals
* Included support for all C-style escape sequences except for octal and hex
* Echoed the way the singleton boolean was handled for singleton empty

Build:

    $ make

Run:

    $ ./ptscheme

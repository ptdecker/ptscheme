/*
 * A basic implementation of Scheme in 'C'
 *
 * LICENSE INFO
 * ------------
 *
 * The licensed libraries used by this project are:
 *    - Brian Fox's GNU Readline under GPL v3 (now maintained by Chet Ramey)
 *           c.f. http://cnswww.cns.cwru.edu/php/chet/readline/rltop.html
 *
 * This applicaiton is based partially upon the shoulders of:
 *    - Daniel Holden's MPC under BSD3
 *           c.f. http://buildyourownlisp.com/
 *           c.f. https://github.com/orangeduck/BuildYourOwnLisp
 *    - Peter Michaux's "Scheme from Scratch"
 *           c.f. http://peter.michaux.ca/articles/scheme-from-scratch-introduction
 *           c.f. https://github.com/petermichaux/bootstrap-scheme
 *          - Changes from the Michaux version:
 *              - Inclusion of an error type so that malformed input doesn't kill the app
 *              - Handled the singleton boolean object type differently to parallel other objects
 *              - Implemented characters using C-style character literals
 *              - Included support for all C-style escape sequences except for octal and hex
 *              - Echoed the way the singleton boolean was handled for singleton empty
 *
 * Due to the inclusion of GNU Readline, this project is also licensed under GPL v3
 */

/* Define directives */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ptscheme.h"
#include "replread.h"
#include "repleval.h"
#include "replprint.h"

/* REPL */

int main(void) {
    printf("ptscheme v0.0.1\n");
    printf("Ctrl-c to exit\n\n");
    while(true) {
        printf("> ");
        write(eval(read(stdin)));
        printf("\n");
    }
    exit(EXIT_SUCCESS);
}

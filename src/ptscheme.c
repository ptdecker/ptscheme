/*
 * A basic implementation of Scheme in 'C'
 *
 * LICENSE INFO
 * ------------
 *
 * This software is licensed under GNU v3 and is based highly upon the works of the authors below.
 * The Michaux scheme is licensesd under GNU v3 too, so this also carries that forward.
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
 *              - Handled 'quote', 'define', 'set', 'ok', and 'if' symbols as singletons too
 */

/* Define directives */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ptscheme.h"
#include "replread.h"
#include "repleval.h"
#include "replprint.h"
#include "environments.h"

object *the_empty_environment;
object *the_global_environment;

/* primitives.c */

#include "memmanager.h"
#include "lisppair.h"
#include "lispint.h"
#include "symbols.h"

object *make_primitive_proc(object *(*fn)(struct object *arguments)) {
    object *obj;
    obj = alloc_object();
    obj->type = PRIMITIVE_PROC;
    obj->data.primitive_proc.fn = fn;
    return obj;
}

bool is_primitive_proc(object *obj) {
    return obj->type == PRIMITIVE_PROC;
}

// Register primitives

void register_primitives() {
    define_variable(make_symbol("+"), make_primitive_proc(add_proc), the_global_environment);
}

// LISP Primitive Procedure: 'add'

object *add_proc(object *arguments) {
    long result = 0;

    while (!is_empty(arguments)) {
        result += (car(arguments))->data.fixnum.value;
        arguments = cdr(arguments);
    }
    return make_fixnum(result);
}

/* REPL */

int main(void) {
    init_environments();
    register_primitives();
    printf("ptscheme v0.0.1\n");
    printf("Ctrl-c to exit\n\n");
    while(true) {
        printf("> ");
        write(eval(read(stdin), the_global_environment));
        printf("\n");
    }
    exit(EXIT_SUCCESS);
}

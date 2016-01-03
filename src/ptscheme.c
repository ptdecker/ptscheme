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
 *              - Broke apart into logical (hopefully) modules instead of one big source file
 *              - Inclusion of an error type so that malformed input doesn't kill the REPL
 *              - Handled the singleton boolean object type differently to parallel other objects
 *              - Implemented characters using C-style character literals
 *              - Included support for all C-style escape sequences except for octal and hex
 *              - Echoed the way the singleton boolean was handled for singleton empty
 *              - Handled 'quote', 'define', 'set', 'ok', 'if', 'lambda' symbols as singletons too
 *              - Avoided goto-based tail recurrsion call approach
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
#include "primitives.h"

object *the_empty_environment;
object *the_global_environment;

/* Lambda support (lambda.c) */

#include "memmanager.h"
#include "lisppair.h"
#include "symbols.h"

object *make_compound_proc(object *parameters, object *body, object* env) {
    object *obj;
    obj = alloc_object();
    obj->type = COMPOUND_PROC;
    obj->data.compound_proc.parameters = parameters;
    obj->data.compound_proc.body = body;
    obj->data.compound_proc.env = env;
    return obj;
}

bool is_compound_proc(object *obj) {
   return obj->type == COMPOUND_PROC;
}

object *make_lambda(object *parameters, object *body) {
    return cons(lambda_symbol(), cons(parameters, body));
}

bool is_lambda(object *exp) {
    return is_tagged_list(exp, lambda_symbol());
}

object *lambda_parameters(object *exp) {
    return cadr(exp);
}

object *lambda_body(object *exp) {
    return cddr(exp);
}

/* REPL */

int main(void) {
    init_environments();
//    register_primitives();
    printf("ptscheme v0.0.1\n");
    printf("Ctrl-c to exit\n\n");
    while(true) {
        printf("> ");
        write(eval(read(stdin), the_global_environment));
        printf("\n");
    }
    exit(EXIT_SUCCESS);
}

/* replread.c */
/* REPL - Read */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//#include <ctype.h>
#include "ptscheme.h"
//#include "memmanager.h"
#include "lispbool.h"
#include "lispchar.h"
#include "lispint.h"
#include "lispstr.h"
#include "lisperr.h"
#include "lisppair.h"
#include "hashtable.h"
#include "symbols.h"
#include "repleval.h"

/* REPL - Evaluate */

//object *eval(object *exp) {
//    return exp;
//}

char is_self_evaluating(object *exp) {
    return is_boolean(exp)   ||
           is_fixnum(exp)    ||
           is_character(exp) ||
           is_empty(exp)     ||
           is_string(exp);
}

char is_tagged_list(object *expression, object *tag) {
    object *the_car;

    if (is_pair(expression)) {
        the_car = car(expression);
        return is_symbol(the_car) && (the_car == tag);
    }
    return false;
}

char is_quoted(object *expression) {
    return is_tagged_list(expression, quote_symbol());
}

object *text_of_quotation(object *exp) {
    return cadr(exp);
}

object *eval(object *exp) {

    if (is_self_evaluating(exp)) {
        return exp;
    }

    if (is_quoted(exp)) {
        return text_of_quotation(exp);
    }

    fprintf(stderr, "cannot eval unknown expression type\n");
    exit(EXIT_FAILURE);
}

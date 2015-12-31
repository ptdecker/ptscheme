/* replread.c */
/* REPL - Read */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ptscheme.h"
#include "lispbool.h"
#include "lispchar.h"
#include "lispint.h"
#include "lispstr.h"
#include "lisperr.h"
#include "lisppair.h"
#include "hashtable.h"
#include "symbols.h"
#include "repleval.h"
#include "environments.h"

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

char is_variable(object *expression) {
    return is_symbol(expression);
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

char is_assignment(object *exp) {
    return is_tagged_list(exp, set_symbol());
}

object *assignment_variable(object *exp) {
//    return car(cdr(exp));
    return cadr(exp);
}

object *assignment_value(object *exp) {
//    return car(cdr(cdr(exp)));
    return caddr(exp);
}

char is_definition(object *exp) {
    return is_tagged_list(exp, define_symbol());
}

object *definition_variable(object *exp) {
    return cadr(exp);
}

object *definition_value(object *exp) {
    return caddr(exp);
}

object *eval_assignment(object *exp, object *env) {
    set_variable_value(assignment_variable(exp), eval(assignment_value(exp), env), env);
    return ok_symbol();
}

object *eval_definition(object *exp, object *env) {
    define_variable(definition_variable(exp), eval(definition_value(exp), env), env);
    return ok_symbol();
}

object *eval(object *exp, object *env) {

    if (is_self_evaluating(exp))
        return exp;

    if (is_variable(exp))
        return lookup_variable_value(exp, env);

    if (is_quoted(exp))
        return text_of_quotation(exp);

    if (is_assignment(exp))
        return eval_assignment(exp, env);

    if (is_definition(exp))
        return eval_definition(exp, env);

    fprintf(stderr, "cannot eval unknown expression type\n");
    exit(EXIT_FAILURE);
}

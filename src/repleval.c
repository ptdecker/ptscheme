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
#include "primitives.h"

#include "replprint.h"

/* REPL - Evaluate */

// Self Evaluating Symbols

bool is_self_evaluating(object *exp) {
    return is_boolean(exp)   ||
           is_fixnum(exp)    ||
           is_character(exp) ||
           is_empty(exp)     ||
           is_string(exp);
}

// LISP Primitive: Variables

bool is_variable(object *expression) {
    return is_symbol(expression);
}

bool is_bound(object *expression, object *env) {
    if (is_variable(expression))
        return !is_error(lookup_variable_value(expression, env));
    return false;
}

// A tagged list is a pair whose car is a specified symbol. The value of
// the tagged list is sthe cdr of the pair
bool is_tagged_list(object *expression, object *tag) {
    object *the_car;
    if (is_pair(expression)) {
        the_car = car(expression);
        return is_symbol(the_car) && (the_car == tag);
    }
    return false;
}

// LISP Primitive: 'quote'
// Note: The single quote symbol ("\'") syntactic sugar is handled in by read()

bool is_quoted(object *expression) {
    return is_tagged_list(expression, quote_symbol());
}

object *text_of_quotation(object *exp) {
    return cadr(exp);
}

// LISP Primitive: assignment (or 'set!')

bool is_assignment(object *exp) {
    return is_tagged_list(exp, set_symbol());
}

object *assignment_variable(object *exp) {
    return cadr(exp);
}

object *assignment_value(object *exp) {
    return caddr(exp);
}

// LISP Primitive: 'define'

bool is_definition(object *exp) {
    return is_tagged_list(exp, define_symbol());
}

object *definition_variable(object *exp) {
    return cadr(exp);
}

object *definition_value(object *exp) {
    return caddr(exp);
}

// LISP Primitive 'if'

bool is_if(object *exp) {
    return is_tagged_list(exp, if_symbol());
}

object *if_predicate(object *exp) {
    return cadr(exp);
}

object *if_consequent(object *exp) {
    return caddr(exp);
}

object *if_alternative(object *exp) {

    if (is_empty(cdddr(exp)))
        return make_boolean(false);

    return cadddr(exp);
}

// Handle Registered Built-in Primitive Procedures

bool is_application(object *exp) {
    return is_pair(exp);
}

object *operator(object *exp) {
    return car(exp);
}

object *operands(object *exp) {
    return cdr(exp);
}

bool is_no_operands(object *ops) {
    return is_empty(ops);
}

object *first_operand(object *ops) {
    return car(ops);
}

object *rest_operands(object *ops) {
    return cdr(ops);
}

object *list_of_values(object *exps, object *env) {
    if (is_no_operands(exps))
        return empty_list();
    return cons(eval(first_operand(exps), env), list_of_values(rest_operands(exps), env));
}

// START RECURSIVE EVAL

object *eval_assignment(object *exp, object *env) {
    if (is_bound(exp, env)) {
        set_variable_value(assignment_variable(exp), eval(assignment_value(exp), env), env);
        return ok_symbol();
    }
    return make_error(50, "unbound variable");
}

object *eval_definition(object *exp, object *env) {
    define_variable(definition_variable(exp), eval(definition_value(exp), env), env);
    return ok_symbol();
}

object *eval(object *exp, object *env) {

    object *procedure;
    object *arguments;

tailcall:
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

    if (is_if(exp)) {
        exp = is_true(eval(if_predicate(exp), env)) ? if_consequent(exp) : if_alternative(exp);
        goto tailcall;
    }

    if (is_application(exp)) {
        if (is_bound(operator(exp), env)) {
            procedure = eval(operator(exp), env);
            arguments = list_of_values(operands(exp), env);
            return (procedure->data.primitive_proc.fn)(arguments);
        }

        //TODO:  This was added so that pairs still properly evaluate and do not return an error;
        //       however; this may not be valid in the final implementation since "symbols are not
        //       self evaluating".  Revisit.
        if (is_pair(exp))
            return exp;

        return make_error(99, "unbound operator");
    }

    fprintf(stderr, "cannot eval unknown expression type\n");
    exit(EXIT_FAILURE);
}

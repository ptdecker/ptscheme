/* repleval.c */

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
#include "repleval.h"

// Self Evaluating Symbols

bool is_self_evaluating(object *exp) {
    return is_boolean(exp)   ||
           is_fixnum(exp)    ||
           is_character(exp) ||
           is_empty(exp)     ||
           is_string(exp);
}

// LISP Primitive: Variables Helper Functions

bool is_variable(object *expression) {
    return is_symbol(expression);
}

bool is_bound(object *expression, object *env) {
    return is_variable(expression) ? !is_error(lookup_variable_value(expression, env)) : false;
}

// A tagged list is a pair whose car is a specified symbol. The value of
// the tagged list is the cdr of the pair
bool is_tagged_list(object *expression, object *tag) {
    object *the_car;
    if (!is_pair(expression))
        return false;

    the_car = car(expression);
    return is_symbol(the_car) && (the_car == tag);
}

// LISP Primitive: 'quote' Helper Functions
// Note: The single quote symbol ("\'") syntactic sugar is handled in by read()

bool is_quoted(object *expression) {
    return is_tagged_list(expression, quote_symbol());
}

object *text_of_quotation(object *exp) {
    return cadr(exp);
}

// LISP Primitive: assignment (or 'set!') Helper Functions

bool is_assignment(object *exp) {
    return is_tagged_list(exp, set_symbol());
}

object *assignment_variable(object *exp) {
    return cadr(exp);
}

object *assignment_value(object *exp) {
    return caddr(exp);
}

// LISP Primitive: 'define' Helper Functions

bool is_definition(object *exp) {
    return is_tagged_list(exp, define_symbol());
}

object *definition_variable(object *exp) {
    return is_symbol(cadr(exp)) ? cadr(exp) : caadr(exp);
}

object *definition_value(object *exp) {
    return is_symbol(cadr(exp)) ? caddr(exp) : make_lambda(cdadr(exp), cddr(exp));
}

// LISP Primitive 'cond' Helper Functions

object *make_if(object *predicate, object *consequent, object *alternative) {
    return
        cons(if_symbol(),
            cons(predicate,
                cons(consequent,
                    cons(alternative, empty_list()))));
}

char is_cond(object *exp) {
    return is_tagged_list(exp, cond_symbol());
}

object *cond_clauses(object *exp) {
    return cdr(exp);
}

object *cond_predicate(object *clause) {
    return car(clause);
}

object *cond_actions(object *clause) {
    return cdr(clause);
}

char is_cond_else_clause(object *clause) {
    return cond_predicate(clause) == else_symbol();
}

object *sequence_to_exp(object *seq) {
    return is_empty(seq) ? seq : (is_last_exp(seq) ? first_exp(seq) : make_begin(seq));
}

object *expand_clauses(object *clauses) {
    object *first;
    object *rest;

    if (is_empty(clauses))
        return false;

    first = car(clauses);
    rest  = cdr(clauses);

    if (!is_cond_else_clause(first))
        return make_if(cond_predicate(first), sequence_to_exp(cond_actions(first)), expand_clauses(rest));

    if (is_empty(rest))
        return sequence_to_exp(cond_actions(first));

    fprintf(stderr, "else clause isn't last cond->if");
    exit(EXIT_FAILURE);
}

object *cond_to_if(object *exp) {
    return expand_clauses(cond_clauses(exp));
}

// LISP Primitive 'if' Helper Functions

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
    return is_empty(cdddr(exp)) ? make_boolean(false) : cadddr(exp);
}

// LISP Primitive 'begin' Helper Functions

object *make_begin(object *exp) {
    return cons(begin_symbol(), exp);
}

bool is_begin(object *exp) {
    return is_tagged_list(exp, begin_symbol());
}

object *begin_actions(object *exp) {
    return cdr(exp);
}

// LISP Primitive 'application' (i.e. apply) Helper Functions

object *make_application(object *operator, object *operands) {
    return cons(operator, operands);
}

bool is_application(object *exp) {
    return is_pair(exp);
}

bool is_last_exp(object *seq) {
    return is_empty(cdr(seq));
}

object *first_exp(object *seq) {
    return car(seq);
}

object *rest_exps(object *seq) {
    return cdr(seq);
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
    return is_no_operands(exps) ?
        empty_list() :
        cons(eval(first_operand(exps), env), list_of_values(rest_operands(exps), env));
}

// LISP Primitive 'let' Helper Functions

char is_let(object *exp) {
    return is_tagged_list(exp, let_symbol());
}

object *let_bindings(object *exp) {
    return cadr(exp);
}

object *let_body(object *exp) {
    return cddr(exp);
}

object *binding_parameter(object *binding) {
    return car(binding);
}

object *binding_argument(object *binding) {
    return cadr(binding);
}

object *bindings_parameters(object *bindings) {
    return is_empty(bindings) ?
               empty_list() :
               cons(binding_parameter(car(bindings)), bindings_parameters(cdr(bindings)));
}

object *bindings_arguments(object *bindings) {
    return is_empty(bindings) ?
               empty_list() :
               cons(binding_argument(car(bindings)), bindings_arguments(cdr(bindings)));
}

object *let_parameters(object *exp) {
    return bindings_parameters(let_bindings(exp));
}

object *let_arguments(object *exp) {
    return bindings_arguments(let_bindings(exp));
}

object *let_to_application(object *exp) {
    return make_application(make_lambda(let_parameters(exp), let_body(exp)), let_arguments(exp));
}

// LISP Primitive 'and' helper functions

bool is_and(object *exp) {
    return is_tagged_list(exp, and_symbol());
}

object *and_tests(object *exp) {
    return cdr(exp);
}

// LISP Primitive 'or' helper functions

bool is_or(object *exp) {
    return is_tagged_list(exp, or_symbol());
}

object *or_tests(object *exp) {
    return cdr(exp);
}

// START RECURSIVE EVAL

object *eval_assignment(object *exp, object *env) {
        set_variable_value(assignment_variable(exp), eval(assignment_value(exp), env), env);
        return ok_symbol();
}

object *eval_definition(object *exp, object *env) {
    define_variable(definition_variable(exp), eval(definition_value(exp), env), env);
    return ok_symbol();
}

object *eval(object *exp, object *env) {

    object *procedure;
    object *arguments;
    object *result;
    bool tailcall = false;

    do {

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
            tailcall = true;
            continue;
        }

        if (is_lambda(exp))
            return make_compound_proc(lambda_parameters(exp), lambda_body(exp), env);

        if (is_begin(exp)) {
            exp = begin_actions(exp);
            while (!is_last_exp(exp)) {
                eval(first_exp(exp), env);
                exp = rest_exps(exp);
            }
            exp = first_exp(exp);
            tailcall = true;
            continue;
        }

        if (is_cond(exp)) {
            exp = cond_to_if(exp);
            tailcall = true;
            continue;
        }

        if (is_let(exp)) {
            exp = let_to_application(exp);
            tailcall = true;
            continue;
        }

        if (is_and(exp)) {
            exp = and_tests(exp);
            if (is_empty(exp))
                 return make_boolean(true);
            while (!is_last_exp(exp)) {
                result = eval(first_exp(exp), env);
                if (is_false(result))
                    return result;
                exp = rest_exps(exp);
            }
            exp = first_exp(exp);
            tailcall = true;
            continue;
        }

        if (is_or(exp)) {
            exp = or_tests(exp);
            if (is_empty(exp)) {
                return make_boolean(false);
            }
            while (!is_last_exp(exp)) {
                result = eval(first_exp(exp), env);
                if (is_true(result))
                    return result;
                exp = rest_exps(exp);
            }
            exp = first_exp(exp);
            tailcall = true;
            continue;
        }

        if (is_application(exp)) {
            procedure = eval(operator(exp), env);
            arguments = list_of_values(operands(exp), env);
            if (is_primitive_proc(procedure))
                return (procedure->data.primitive_proc.fn)(arguments);
            if (is_compound_proc(procedure)) {
                env = extend_environment(procedure->data.compound_proc.parameters, arguments, procedure->data.compound_proc.env);
                exp = make_begin(procedure->data.compound_proc.body);
                tailcall = true;
                continue;
            }
            return make_error(342, "unknown procedure type");
        } // is_application()

    } while (tailcall);

    fprintf(stderr, "cannot eval unknown expression type\n");
    exit(EXIT_FAILURE);
}

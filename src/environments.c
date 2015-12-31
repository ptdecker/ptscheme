/* environments.c */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "ptscheme.h"
#include "lisppair.h"
#include "lisperr.h"
#include "environments.h"

object *enclosing_environment(object *env) {
    return cdr(env);
}

object *first_frame(object *env) {
    return car(env);
}

object *make_frame(object *variables, object *values) {
    return cons(variables, values);
}

object *frame_variables(object *frame) {
    return car(frame);
}

object *frame_values(object *frame) {
    return cdr(frame);
}

void add_binding_to_frame(object *var, object *val, object *frame) {
    set_car(frame, cons(var, car(frame)));
    set_cdr(frame, cons(val, cdr(frame)));
}

object *extend_environment(object *vars, object *vals, object *base_env) {
    return cons(make_frame(vars, vals), base_env);
}

object *lookup_variable_value(object *var, object *env) {
    object *frame;
    object *vars;
    object *vals;
    while (!is_empty(env)) {
        frame = first_frame(env);
        vars  = frame_variables(frame);
        vals  = frame_values(frame);
        while (!is_empty(vars)) {
            if (var == car(vars)) {
                return car(vals);
            }
            vars = cdr(vars);
            vals = cdr(vals);
        }
        env = enclosing_environment(env);
    }
    return make_error(50, "unbound variable");
}

//TODO: set_variable_value needs to soft receover
void set_variable_value(object *var, object *val, object *env) {
    object *frame;
    object *vars;
    object *vals;

    while (!is_empty(env)) {
        frame = first_frame(env);
        vars = frame_variables(frame);
        vals = frame_values(frame);
        while (!is_empty(vars)) {
            if (var == car(vars)) {
                set_car(vals, val);
                return;
            }
            vars = cdr(vars);
            vals = cdr(vals);
        }
        env = enclosing_environment(env);
    }
    fprintf(stderr, "unbound variable\n");
    exit(EXIT_FAILURE);
}

void define_variable(object *var, object *val, object *env) {
    object *frame;
    object *vars;
    object *vals;

    frame = first_frame(env);
    vars = frame_variables(frame);
    vals = frame_values(frame);

    while (!empty_list(vars)) {
        if (var == car(vars)) {
            set_car(vals, val);
            return;
        }
        vars = cdr(vars);
        vals = cdr(vals);
    }
    add_binding_to_frame(var, val, frame);
}

object *setup_environment(void) {
    object *initial_env;

    initial_env = extend_environment(empty_list(), empty_list(), the_empty_environment);

    return initial_env;
}

void init_environments() {
    the_empty_environment  = empty_list();
    the_global_environment = setup_environment();
}

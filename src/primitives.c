/* primitives.c */

#include <stdbool.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include "ptscheme.h"
#include "memmanager.h"
#include "lisppair.h"
#include "lispint.h"
#include "lispbool.h"
#include "lispchar.h"
#include "lispstr.h"
#include "lisperr.h"
//#include "hashtable.h"
//#include "repleval.h"
#include "symbols.h"
#include "environments.h"

object *make_primitive_proc(object *(*fn)(struct object *arguments)) {
    object *obj;
    obj = alloc_object();
    obj->type = PRIMITIVE_PROC;
    obj->data.primitive_proc.fn = fn;
    return obj;
}

// Register primitives

bool is_primitive_proc(object *obj) {
    return obj->type == PRIMITIVE_PROC;
}

object *is_null_proc(object *arguments) {
    return is_empty(car(arguments)) ? make_boolean(true) : make_boolean(false);
}

object *is_boolean_proc(object *arguments) {
    return is_boolean(car(arguments)) ? make_boolean(true) : make_boolean(false);
}

object *is_symbol_proc(object *arguments) {
    return is_symbol(car(arguments)) ? make_boolean(true) : make_boolean(false);
}

object *is_integer_proc(object *arguments) {
    return is_fixnum(car(arguments)) ? make_boolean(true) : make_boolean(false);
}

object *is_char_proc(object *arguments) {
    return is_character(car(arguments)) ? make_boolean(true) : make_boolean(false);
}

object *is_string_proc(object *arguments) {
    return is_string(car(arguments)) ? make_boolean(true) : make_boolean(false);
}

object *is_pair_proc(object *arguments) {
    return is_pair(car(arguments)) ? make_boolean(true) : make_boolean(false);
}

object *is_procedure_proc(object *arguments) {
    return is_primitive_proc(car(arguments)) ? make_boolean(true) : make_boolean(false);
}

object *char_to_integer_proc(object *arguments) {
    return make_fixnum((car(arguments))->data.character.value);
}

object *integer_to_char_proc(object *arguments) {
    return make_character((car(arguments))->data.fixnum.value);
}

object *number_to_string_proc(object *arguments) {
    char buffer[100];

    sprintf(buffer, "%ld", (car(arguments))->data.fixnum.value);
    return make_string(buffer);
}

object *string_to_number_proc(object *arguments) {
    return make_fixnum(atoi((car(arguments))->data.string.value));
}

object *symbol_to_string_proc(object *arguments) {
    return make_string((car(arguments))->data.symbol.value);
}

object *string_to_symbol_proc(object *arguments) {
    return make_symbol((car(arguments))->data.string.value);
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

object *sub_proc(object *arguments) {
    long result;

    result = (car(arguments))->data.fixnum.value;
    while (!is_empty(arguments = cdr(arguments))) {
        result -= (car(arguments))->data.fixnum.value;
    }
    return make_fixnum(result);
}

object *mul_proc(object *arguments) {
    long result = 1;

    while (!is_empty(arguments)) {
        result *= (car(arguments))->data.fixnum.value;
        arguments = cdr(arguments);
    }
    return make_fixnum(result);
}

object *quotient_proc(object *arguments) {
    return make_fixnum(
            ((car(arguments) )->data.fixnum.value) /
            ((cadr(arguments))->data.fixnum.value)
        );
}

object *remainder_proc(object *arguments) {
    return make_fixnum(
            ((car(arguments) )->data.fixnum.value) %
            ((cadr(arguments))->data.fixnum.value)
        );
}

object *is_number_equal_proc(object *arguments) {
    long value;

    value = (car(arguments))->data.fixnum.value;
    while (!is_empty(arguments = cdr(arguments)))
        if (value != ((car(arguments))->data.fixnum.value))
            return make_boolean(false);

    return make_boolean(true);
}

object *is_less_than_proc(object *arguments) {
    long previous;
    long next;

    previous = (car(arguments))->data.fixnum.value;
    while (!is_empty(arguments = cdr(arguments))) {
        next = (car(arguments))->data.fixnum.value;
        if (previous < next) {
            previous = next;
        } else {
            return make_boolean(false);
        }
    }
    return make_boolean(true);
}

object *is_greater_than_proc(object *arguments) {
    long previous;
    long next;

    previous = (car(arguments))->data.fixnum.value;
    while (!is_empty(arguments = cdr(arguments))) {
        next = (car(arguments))->data.fixnum.value;
        if (previous > next) {
            previous = next;
        } else {
            return make_boolean(false);
        }
    }
    return make_boolean(true);
}

object *cons_proc(object *arguments) {
    return cons(car(arguments), cadr(arguments));
}

object *car_proc(object *arguments) {
    return caar(arguments);
}

object *cdr_proc(object *arguments) {
    return cdar(arguments);
}

object *set_car_proc(object *arguments) {
    set_car(car(arguments), cadr(arguments));
    return ok_symbol();
}

object *set_cdr_proc(object *arguments) {
    set_cdr(car(arguments), cadr(arguments));
    return ok_symbol();
}

object *list_proc(object *arguments) {
    return arguments;
}

object *is_eq_proc(object *arguments) {
    object *obj1;
    object *obj2;

    obj1 = car(arguments);
    obj2 = cadr(arguments);

    if (obj1->type != obj2->type)
        return make_boolean(false);


    //TODO: Still smells
    switch (obj1->type) {
        case FIXNUM:
            return make_boolean((obj1->data.fixnum.value == obj2->data.fixnum.value) ? true : false);
            break;
        case CHARACTER:
            return make_boolean((obj1->data.character.value == obj2->data.character.value) ? true : false);
            break;
        case STRING:
            return make_boolean((strcmp(obj1->data.string.value, obj2->data.string.value) == 0) ? true : false);
            break;
        default:
            return make_boolean((obj1 == obj2) ? true : false);
    }
}

#define add_procedure(scheme_name, c_name)       \
    define_variable(make_symbol(scheme_name), make_primitive_proc(c_name), the_global_environment);

void register_primitives() {

    add_procedure("null?"     , is_null_proc);
    add_procedure("boolean?"  , is_boolean_proc);
    add_procedure("symbol?"   , is_symbol_proc);
    add_procedure("integer?"  , is_integer_proc);
    add_procedure("char?"     , is_char_proc);
    add_procedure("string?"   , is_string_proc);
    add_procedure("pair?"     , is_pair_proc);
    add_procedure("procedure?", is_procedure_proc);

    add_procedure("char->integer" , char_to_integer_proc);
    add_procedure("integer->char" , integer_to_char_proc);
    add_procedure("number->string", number_to_string_proc);
    add_procedure("string->number", string_to_number_proc);
    add_procedure("symbol->string", symbol_to_string_proc);
    add_procedure("string->symbol", string_to_symbol_proc);

    add_procedure("+"        , add_proc);
    add_procedure("-"        , sub_proc);
    add_procedure("*"        , mul_proc);
    add_procedure("quotient" , quotient_proc);
    add_procedure("remainder", remainder_proc);
    add_procedure("="        , is_number_equal_proc);
    add_procedure("<"        , is_less_than_proc);
    add_procedure(">"        , is_greater_than_proc);

    add_procedure("cons"    , cons_proc);
    add_procedure("car"     , car_proc);
    add_procedure("cdr"     , cdr_proc);
    add_procedure("set-car!", set_car_proc);
    add_procedure("set-cdr!", set_cdr_proc);
    add_procedure("list"    , list_proc);

    add_procedure("eq?", is_eq_proc);
}

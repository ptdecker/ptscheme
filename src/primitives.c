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
#include "symbols.h"
#include "environments.h"

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

// LISP Primitive Procedure: 'null?'

object *is_null_proc(object *arguments) {
    return make_boolean(is_empty(car(arguments)));
}

// LISP Primitive Procedure: 'boolean?'

object *is_boolean_proc(object *arguments) {
    return make_boolean(is_boolean(car(arguments)));
}

// LISP Primitive Procedure: 'symbol?'

object *is_symbol_proc(object *arguments) {
    return make_boolean(is_symbol(car(arguments)));
}

// LISP Primitive Procedure: 'integer?'

object *is_integer_proc(object *arguments) {
    return make_boolean(is_fixnum(car(arguments)));
}

// LISP Primitive Procedure: 'char?'

object *is_char_proc(object *arguments) {
    return make_boolean(is_character(car(arguments)));
}

// LISP Primitive Procedure: 'string?'

object *is_string_proc(object *arguments) {
    return make_boolean(is_string(car(arguments)));
}

// LISP Primitive Procedure: 'pair?'

object *is_pair_proc(object *arguments) {
    return make_boolean(is_pair(car(arguments)));
}

// LISP Primitive Procedure: 'procedure?'

object *is_procedure_proc(object *arguments) {
    object *obj;
    obj = car(arguments);
    return make_boolean((is_primitive_proc(obj) || is_compound_proc(obj)));
}

// LISP Primitive Procedure: 'char->integer'

object *char_to_integer_proc(object *arguments) {
    return make_fixnum((car(arguments))->data.character.value);
}

// LISP Primitive Procedure: 'integer->char'

object *integer_to_char_proc(object *arguments) {
    return make_character((car(arguments))->data.fixnum.value);
}

// LISP Primitive Procedure: 'number->string'

object *number_to_string_proc(object *arguments) {
    char buffer[100];

    sprintf(buffer, "%ld", (car(arguments))->data.fixnum.value);
    return make_string(buffer);
}

// LISP Primitive Procedure: 'string->number'

object *string_to_number_proc(object *arguments) {
    return make_fixnum(atoi((car(arguments))->data.string.value));
}

// LISP Primitive Procedure: 'symbol->string'

object *symbol_to_string_proc(object *arguments) {
    return make_string((car(arguments))->data.symbol.value);
}

// LISP Primitive Procedure: 'string->symbol'

object *string_to_symbol_proc(object *arguments) {
    return make_symbol((car(arguments))->data.string.value);
}

// LISP Primitive Procedure: '+'

object *add_proc(object *arguments) {
    long result = 0;

    while (!is_empty(arguments)) {
        result += (car(arguments))->data.fixnum.value;
        arguments = cdr(arguments);
    }
    return make_fixnum(result);
}

// LISP Primitive Procedure: '-'

object *sub_proc(object *arguments) {
    long result;

    result = (car(arguments))->data.fixnum.value;
    while (!is_empty(arguments = cdr(arguments))) {
        result -= (car(arguments))->data.fixnum.value;
    }
    return make_fixnum(result);
}

// LISP Primitive Procedure: '*'

object *mul_proc(object *arguments) {
    long result = 1;

    while (!is_empty(arguments)) {
        result *= (car(arguments))->data.fixnum.value;
        arguments = cdr(arguments);
    }
    return make_fixnum(result);
}

// LISP Primitive Procedure: 'quotient'

object *quotient_proc(object *arguments) {
    return make_fixnum(
            ((car(arguments) )->data.fixnum.value) /
            ((cadr(arguments))->data.fixnum.value)
        );
}

// LISP Primitive Procedure: 'remainder'

object *remainder_proc(object *arguments) {
    return make_fixnum(
            ((car(arguments) )->data.fixnum.value) %
            ((cadr(arguments))->data.fixnum.value)
        );
}

// LISP Primitive Procedure: '=' (numeric only)

object *is_number_equal_proc(object *arguments) {
    long value;

    value = (car(arguments))->data.fixnum.value;
    while (!is_empty(arguments = cdr(arguments)))
        if (value != ((car(arguments))->data.fixnum.value))
            return make_boolean(false);

    return make_boolean(true);
}

// LISP Primitive Procedure: '<'

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

// LISP Primitive Procedure: '>'

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

// LISP Primitive Procedure: 'cons'

object *cons_proc(object *arguments) {
    return cons(car(arguments), cadr(arguments));
}

// LISP Primitive Procedure: 'car'

object *car_proc(object *arguments) {
    return caar(arguments);
}

// LISP Primitive Procedure: 'cdr'

object *cdr_proc(object *arguments) {
    return cdar(arguments);
}

// LISP Primitive Procedure: 'set-car!'

object *set_car_proc(object *arguments) {
    set_car(car(arguments), cadr(arguments));
    return ok_symbol();
}

// LISP Primitive Procedure: 'set-cdr!'

object *set_cdr_proc(object *arguments) {
    set_cdr(car(arguments), cadr(arguments));
    return ok_symbol();
}

// LISP Primitive Procedure: 'list'

object *list_proc(object *arguments) {
    return arguments;
}

// LISP Primitive Procedure: 'eq?'

object *is_eq_proc(object *arguments) {
    object *obj1;
    object *obj2;

    obj1 = car(arguments);
    obj2 = cadr(arguments);

    if (obj1->type != obj2->type)
        return make_boolean(false);

    //TODO: Should eq? recurse so that, for example, (eq? (cons 3 4) (cons 3 4)) returns true?  Use eval? eval + write?
    switch (obj1->type) {
        case FIXNUM:
            return make_boolean(obj1->data.fixnum.value == obj2->data.fixnum.value);
            break;
        case CHARACTER:
            return make_boolean(obj1->data.character.value == obj2->data.character.value);
            break;
        case STRING:
            return make_boolean(strcmp(obj1->data.string.value, obj2->data.string.value) == 0);
            break;
        default:
            return make_boolean(obj1 == obj2);
    }
}

// LISP Primitive Procedure: 'apply'

object *apply_proc(object *arguments) {
    fprintf(stderr, "illegal state: The body of the apply "
            "primitive procedure should not execute.\n");
    exit(EXIT_FAILURE);
}

// Macro definition for registering a primitive procedure

#define add_procedure(scheme_name, c_name)       \
    define_variable(make_symbol(scheme_name), make_primitive_proc(c_name), the_global_environment);

// Register all the primitive procedures

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

    add_procedure("apply", apply_proc);
}

/* primitives.c */

#include <stdbool.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ptscheme.h"
#include "memmanager.h"
#include "lisppair.h"
#include "lispint.h"
#include "lispfloat.h"
#include "lispbool.h"
#include "lispchar.h"
#include "lispstr.h"
#include "lisperr.h"
#include "symbols.h"
#include "environments.h"
#include "replread.h"
#include "repleval.h"
#include "replprint.h"

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

// LISP Primitive Procedure: 'real?'

object *is_real_proc(object *arguments) {
    return make_boolean(is_floatnum(car(arguments)));
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
    long   result = 0;
    double result2 = 0.0;
    bool   real = false;

    while (!is_empty(arguments)) {
        real = is_floatnum(car(arguments));
        if (real)
            result2 += (car(arguments))->data.floatnum.value;
        else
            result += (car(arguments))->data.fixnum.value;
        arguments = cdr(arguments);
    }

    //TODO: Consider using fpisanint() instead of fmod approach
    if ((real || (result2 != 0.0)) && fmod(result2, 1) != 0.0)
        return make_floatnum(result2 + (double)result);
    else if (real || (result2 != 0.0))
        return make_fixnum(lround(result2 + (double)result));
    else
        return make_fixnum(result);
}

// LISP Primitive Procedure: '-'

object *sub_proc(object *arguments) {
    long result;
    double result2 = 0.0;
    bool   real_mode = false;
    bool   real = false;

    real = is_floatnum(car(arguments));
    if (real) {
        real_mode = true;
        result2 = (car(arguments))->data.floatnum.value;
    } else
        result = (car(arguments))->data.fixnum.value;

    while (!is_empty(arguments = cdr(arguments))) {
        if (real_mode) {
            real = is_floatnum(car(arguments));
            if (real)
                result2 -= (car(arguments))->data.floatnum.value;
            else
                result2 -= (double)(car(arguments))->data.fixnum.value;
        } else {
            real = is_floatnum(car(arguments));
            if (real) {
                result2 = (double)result - (car(arguments))->data.floatnum.value;
                real_mode = true;
            } else
                result -= (car(arguments))->data.fixnum.value;
        }
    }

    //TODO: Consider using fpisanint() instead of fmod approach
    if (real_mode && fmod(result2, 1) != 0.0)
        return make_floatnum(result2);
    else if (real_mode)
        return make_fixnum(lround(result2));
    else
        return make_fixnum(result);
}

// LISP Primitive Procedure: '*'

object *mul_proc(object *arguments) {
    long   result = 1;
    double result2 = 1.0;
    bool   real = false;

    while (!is_empty(arguments)) {
        real = is_floatnum(car(arguments));
        if (real)
            result2 *= (car(arguments))->data.floatnum.value;
        else
            result *= (car(arguments))->data.fixnum.value;
        arguments = cdr(arguments);
    }

    //TODO: Consider using fpisanint() instead of fmod approach
    if ((real || (result2 != 0.0)) && fmod(result2, 1) != 0.0)
        return make_floatnum(result2 * (double)result);
    else if (real || (result2 != 0.0))
        return make_fixnum(lround(result2 * (double)result));
    else
        return make_fixnum(result);
}

// LISP Primitive Procedure: 'quotient'

//TODO: Add integer reduction, consider using fpisanint() instead of fmod approach
object *quotient_proc(object *arguments) {
    if (is_floatnum(car(arguments)) || is_floatnum(cadr(arguments))) {
        return make_floatnum(
                (is_floatnum(car(arguments))  ? (car(arguments))->data.floatnum.value  : (double)(car(arguments))->data.fixnum.value) /
                (is_floatnum(cadr(arguments)) ? (cadr(arguments))->data.floatnum.value : (double)(cadr(arguments))->data.fixnum.value)
            );
    } else {
        return make_fixnum(
                ((car(arguments) )->data.fixnum.value) /
                ((cadr(arguments))->data.fixnum.value)
            );
    }

}

// LISP Primitive Procedure: 'remainder'

//TODO: Add integer reduction, consider using fpisanint() instead of fmod approach
object *remainder_proc(object *arguments) {
    if (is_floatnum(car(arguments)) || is_floatnum(cadr(arguments))) {
        return make_floatnum(
                fmod((is_floatnum(car(arguments))  ? (car(arguments))->data.floatnum.value  : (double)(car(arguments))->data.fixnum.value),
                     (is_floatnum(cadr(arguments)) ? (cadr(arguments))->data.floatnum.value : (double)(cadr(arguments))->data.fixnum.value))
            );
    } else {
        return make_fixnum(
                ((car(arguments) )->data.fixnum.value) %
                ((cadr(arguments))->data.fixnum.value)
            );
    }
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
    fprintf(stderr, "illegal state: The body of the apply primitive procedure should not execute.\n");
    exit(EXIT_FAILURE);
}

// LISP Primitive Procedure 'eval'

object *eval_proc(object *arguments) {
    fprintf(stderr, "illegal state: The body of the eval primitive procedure should not execute.\n");
    exit(EXIT_FAILURE);
}

object *interaction_environment_proc(object *arguments) {
    return the_global_environment;
}

object *null_environment_proc(object *arguments) {
    return setup_environment();
}

object *environment_proc(object *arguments) {
     return make_environment();
}

// ptscheme Primitive Procedure 'exit'

object *exit_proc(object *arguments) {
    if (is_empty(arguments))
        exit(EXIT_SUCCESS);
    if (is_fixnum(car(arguments)))
        exit(car(arguments)->data.fixnum.value);
    if (is_string(car(arguments)))
        fprintf(stderr, "Terminal Error: \"%s\"\n", car(arguments)->data.string.value);
    exit(EXIT_FAILURE);
}

// LISP Primitive Procedures for IO system

object *load_proc(object *arguments) {
    char *filename;
    FILE *in;
    object *exp;
    object *result;

    filename = car(arguments)->data.string.value;
    in = fopen(filename, "r");

    if (in == NULL)
        return make_error(88, "could not load file");

    while ((exp = read(in)) != NULL)
        result = eval(exp, the_global_environment);

    fclose(in);
    return result;
}

object *make_input_port(FILE *stream) {
    object *obj;
    obj = alloc_object();
    obj->type = INPUT_PORT;
    obj->data.input_port.stream = stream;
    return obj;
}

bool is_input_port(object *obj) {
    return obj->type == INPUT_PORT;
}

object *make_output_port(FILE *stream) {
    object *obj;
    obj = alloc_object();
    obj->type = OUTPUT_PORT;
    obj->data.output_port.stream = stream;
    return obj;
}

bool is_output_port(object *obj) {
    return obj->type == OUTPUT_PORT;
}

bool is_eof_object(object *obj) {
    return obj == eof_object;
}

object *open_input_port_proc(object *arguments) {
    char *filename;
    FILE *in;
    filename = car(arguments)->data.string.value;
    in = fopen(filename, "r");
    if (in == NULL) {
        fprintf(stderr, "could not open file \"%s\"\n", filename);
        exit(EXIT_FAILURE);
    }
    return make_input_port(in);
}

object *close_input_port_proc(object *arguments) {
    int result;
    result = fclose(car(arguments)->data.input_port.stream);
    if (result == EOF) {
        fprintf(stderr, "could not close input port\n");
        exit(EXIT_FAILURE);
    }
    return ok_symbol();
}

object *is_input_port_proc(object *arguments) {
    return make_boolean(is_input_port(car(arguments)));
}

object *open_output_port_proc(object *arguments) {
    char *filename;
    FILE *out;
    filename = car(arguments)->data.string.value;
    out = fopen(filename, "w");
    if (out == NULL) {
        fprintf(stderr, "could not open file \"%s\"\n", filename);
        exit(EXIT_FAILURE);
    }
    return make_output_port(out);
}

object *close_output_port_proc(object *arguments) {
    int result;
    result = fclose(car(arguments)->data.output_port.stream);
    if (result == EOF) {
        fprintf(stderr, "could not close output port\n");
        exit(EXIT_FAILURE);
    }
    return ok_symbol();
}

object *is_output_port_proc(object *arguments) {
    return make_boolean(is_output_port(car(arguments)));
}

object *write_char_proc(object *arguments) {
    object *character;
    FILE *out;
    character = car(arguments);
    arguments = cdr(arguments);
    out = is_empty(arguments) ?
             stdout :
             car(arguments)->data.output_port.stream;
    putc(character->data.character.value, out);
    fflush(out);
    return ok_symbol();
}

object *is_eof_object_proc(object *arguments) {
    return make_boolean(is_eof_object(car(arguments)));
}

object *error_proc(object *arguments) {
    while (!is_empty(arguments)) {
        write(stderr, car(arguments));
        fprintf(stderr, " ");
        fputc('\n', stderr);
        arguments = cdr(arguments);
    };
    exit(EXIT_FAILURE);
}

object *read_char_proc(object *arguments) {
    FILE *in;
    int result;
    in = is_empty(arguments) ?
             stdin :
             car(arguments)->data.input_port.stream;
    result = getc(in);
    return (result == EOF) ? eof_object : make_character(result);
}

object *peek_char_proc(object *arguments) {
    FILE *in;
    int result;
    in = is_empty(arguments) ?
             stdin :
             car(arguments)->data.input_port.stream;
    result = peek(in);
    return (result == EOF) ? eof_object : make_character(result);
}

// Lisp Primitive Procedure: 'write'

object *write_proc(object *arguments) {
    object *exp;
    FILE *out;

    exp = car(arguments);
    arguments = cdr(arguments);
    out = is_empty(arguments) ?
             stdout :
             car(arguments)->data.output_port.stream;
    write(out, exp);
    fflush(out);
    return ok_symbol();
}

// Lisp Primitive Procedure: 'read'

object *read_proc(object *arguments) {
    FILE *in;
    object *result;
    in = is_empty(arguments) ?
             stdin :
             car(arguments)->data.input_port.stream;
    result = read(in);
    return (result == NULL) ? eof_object : result;
}

// Lisp Primitive Procedure: 'display'

object *display_proc(object *arguments) {

    object *obj;
    char *str  = NULL;

    obj = car(arguments);

    switch (obj->type) {
        case CHARACTER:
            printf("%c", obj->data.character.value);
            break;
        case FIXNUM:
            printf("%ld", obj->data.fixnum.value);
            break;
        case FLOATNUM:
            printf("%f", obj->data.floatnum.value);
            break;
        case STRING:
            printf("%s", str = obj->data.string.value);
            break;
        case SYMBOL:
            printf("%s", obj->data.symbol.value);
            break;
        case ERROR:
            printf("Error %ld: %s", obj->data.error.error_num, obj->data.error.error_msg);
            break;
        default:
            fprintf(stderr, "cannot display type\n");
    } // switch

    return ok_symbol();

}

// Macro definition for registering a primitive procedure

#define add_procedure(scheme_name, c_name)       \
    define_variable(make_symbol(scheme_name), make_primitive_proc(c_name), env);

// Register all the primitive procedures

void populate_environment(object *env) {

    add_procedure("null?"     , is_null_proc);
    add_procedure("boolean?"  , is_boolean_proc);
    add_procedure("symbol?"   , is_symbol_proc);
    add_procedure("integer?"  , is_integer_proc);
    add_procedure("real?"     , is_real_proc);
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

    add_procedure("interaction-environment", interaction_environment_proc);
    add_procedure("null-environment"       , null_environment_proc);
    add_procedure("environment"            , environment_proc);
    add_procedure("eval"                   , eval_proc);

    add_procedure("exit", exit_proc);

    add_procedure("load"              , load_proc);
    add_procedure("open-input-port"   , open_input_port_proc);
    add_procedure("close-input-port"  , close_input_port_proc);
    add_procedure("eof-object?"       , is_eof_object_proc);
    add_procedure("open-output-port"  , open_output_port_proc);
    add_procedure("close-output-port" , close_output_port_proc);
    add_procedure("input-port?"       , is_input_port_proc);
    add_procedure("output-port?"      , is_output_port_proc);
    add_procedure("write-char"        , write_char_proc);
    add_procedure("error"             , error_proc);
    add_procedure("read-char"         , read_char_proc);
    add_procedure("peek-char"         , peek_char_proc);
    add_procedure("write"             , write_proc);
    add_procedure("read"              , read_proc);

    add_procedure("display"           , display_proc);

}

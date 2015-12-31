/* primitives.c */

#include <stdbool.h>
#include "ptscheme.h"
#include "memmanager.h"
#include "lisppair.h"
#include "lispint.h"
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

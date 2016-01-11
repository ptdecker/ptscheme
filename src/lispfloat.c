/* lispint.c */

#include <stdbool.h>
#include <stddef.h>
#include "ptscheme.h"
#include "memmanager.h"

// Integer native type

object *make_floatnum(double value) {
    object *obj;
    obj = alloc_object();
    obj->type = FLOATNUM;
    obj->data.floatnum.value = value;
    return obj;
}

bool is_floatnum(object *obj) {
    return (obj->type == FLOATNUM);
}

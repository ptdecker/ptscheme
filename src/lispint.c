/* lispint.c */

#include <stdbool.h>
#include <stddef.h>
#include "ptscheme.h"
#include "memmanager.h"

// Integer native type

object *make_fixnum(long value) {
	object *obj;
	obj = alloc_object();
	obj->type = FIXNUM;
	obj->data.fixnum.value = value;
	return obj;
}

bool is_fixnum(object *obj) {
	return (obj->type == FIXNUM);
}

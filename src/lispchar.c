/* lispchar.c */

#include <stdbool.h>
#include <stddef.h>
#include "ptscheme.h"
#include "memmanager.h"

// Character native type

object *make_character(char value) {
	object *obj;
	obj = alloc_object();
	obj->type = CHARACTER;
	obj->data.character.value = value;
	return obj;
}

bool is_character(object *obj) {
	return (obj->type == CHARACTER);
}
/* lispbool.c */

#include <stdbool.h>
#include <stddef.h>
#include "ptscheme.h"
#include "memmanager.h"

// Boolean native type

object *make_boolean(char value) {

	// The boolean objects are singletons

	static object *bool_true;
	static object *bool_false;

	if (bool_true == NULL) {
		bool_true = alloc_object();
		bool_true->type = BOOLEAN;
		bool_true->data.boolean.value = true;
	}
	if (bool_false == NULL) {
		bool_false = alloc_object();
		bool_false->type = BOOLEAN;
		bool_false->data.boolean.value = false;
	}
	return ((value) ? bool_true : bool_false);
}

bool is_boolean(object *obj) {
	return (obj->type == BOOLEAN);
}

bool is_false(object *obj) {
	return (obj->data.boolean.value == false);
}

bool is_true(object *obj) {
	return (obj->data.boolean.value == true);
}


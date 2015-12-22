/* lisperr.c */

#include <stdbool.h>
#include <stddef.h>
#include "ptscheme.h"
#include "memmanager.h"

// Error native type

object *make_error(long error_num, char *error_msg) {
	object *obj;
	obj = alloc_object();
	obj->type = ERROR;
	obj->data.error.error_num = error_num;
	obj->data.error.error_msg = error_msg;
	return obj;
}

bool is_error(object *obj) {
	return (obj->type == ERROR);
}


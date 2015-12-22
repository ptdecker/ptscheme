/* lispstr.c */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ptscheme.h"
#include "memmanager.h"

//TODO: Drop the usage of malloc here and move it into the memory management approach

// String native type

object *make_string(char *value) {
	object *obj;
	obj = alloc_object();
	obj->type = STRING;
	obj->data.string.value = malloc(strlen(value) + 1);
	if (obj->data.string.value == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(obj->data.string.value, value);
	return obj;
}

bool is_string(object *obj) {
	return (obj->type == STRING);
}
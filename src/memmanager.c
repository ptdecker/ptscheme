/* memmanager.c */

#include <stdlib.h>
#include <stdio.h>
#include "ptscheme.h"

/* Lisp object constructors */

object *alloc_object(void) {
	object *obj;
	obj = malloc(sizeof(object));
	if (obj == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}
	return obj;
}
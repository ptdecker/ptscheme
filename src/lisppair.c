/* lisppair.c  */

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "ptscheme.h"
#include "memmanager.h"

/* Empty List */

object *make_empty() {

	// The empty list is a singletons

	static object *empty_list;

	if (empty_list == NULL) {
		empty_list = alloc_object();
		empty_list->type = EMPTY_LIST;
	}

	return empty_list;
}

bool is_empty(object *obj) {
	return (obj->type == EMPTY_LIST);
}

/* LISP Primitive: cons */

object *cons(object *car, object *cdr) {

    object *obj;
    
    obj = alloc_object();
    obj->type = PAIR;
    obj->data.pair.car = car;
    obj->data.pair.cdr = cdr;

    return obj;
}

char is_pair(object *obj) {
    return obj->type == PAIR;
}

/* LISP Primitive: car */

object *car(object *pair) {

	if (!is_pair(pair)) {
		fprintf(stderr, "%p (type: %d) is not a pair\n", pair, pair->type);
    	exit(EXIT_FAILURE);
	}

    return pair->data.pair.car;
}

void set_car(object *obj, object* value) {
    obj->data.pair.car = value;
}

/* LISP Primitive: cdr */

object *cdr(object *pair) {
	
	if (!is_pair(pair)) {
		fprintf(stderr, "%p (type: %d) is not a pair\n", pair, pair->type);
    	exit(EXIT_FAILURE);
	}

    return pair->data.pair.cdr;
}

void set_cdr(object *obj, object* value) {
    obj->data.pair.cdr = value;
}

#define caar(obj)   car(car(obj))
#define cadr(obj)   car(cdr(obj))
#define cdar(obj)   cdr(car(obj))
#define cddr(obj)   cdr(cdr(obj))
#define caaar(obj)  car(car(car(obj)))
#define caadr(obj)  car(car(cdr(obj)))
#define cadar(obj)  car(cdr(car(obj)))
#define caddr(obj)  car(cdr(cdr(obj)))
#define cdaar(obj)  cdr(car(car(obj)))
#define cdadr(obj)  cdr(car(cdr(obj)))
#define cddar(obj)  cdr(cdr(car(obj)))
#define cdddr(obj)  cdr(cdr(cdr(obj)))
#define caaaar(obj) car(car(car(car(obj))))
#define caaadr(obj) car(car(car(cdr(obj))))
#define caadar(obj) car(car(cdr(car(obj))))
#define caaddr(obj) car(car(cdr(cdr(obj))))
#define cadaar(obj) car(cdr(car(car(obj))))
#define cadadr(obj) car(cdr(car(cdr(obj))))
#define caddar(obj) car(cdr(cdr(car(obj))))
#define cadddr(obj) car(cdr(cdr(cdr(obj))))
#define cdaaar(obj) cdr(car(car(car(obj))))
#define cdaadr(obj) cdr(car(car(cdr(obj))))
#define cdadar(obj) cdr(car(cdr(car(obj))))
#define cdaddr(obj) cdr(car(cdr(cdr(obj))))
#define cddaar(obj) cdr(cdr(car(car(obj))))
#define cddadr(obj) cdr(cdr(car(cdr(obj))))
#define cdddar(obj) cdr(cdr(cdr(car(obj))))
#define cddddr(obj) cdr(cdr(cdr(cdr(obj))))


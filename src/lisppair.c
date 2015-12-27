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

    static object *empty_list = NULL;

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

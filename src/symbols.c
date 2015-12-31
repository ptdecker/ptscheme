/* symbols.c */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "ptscheme.h"
#include "memmanager.h"
#include "hashtable.h"
#include "replprint.h"

object *make_symbol(char *value) {
    object *obj;
    // is object already in has table (based upon value)?
    obj = hashget(value);
    // if (not found)
    if (obj == NULL) {
    //      allocate new object
            obj = alloc_object();
            obj->type = SYMBOL;
            obj->data.symbol.value = malloc(strlen(value) + 1);
            if (obj->data.symbol.value == NULL) {
                fprintf(stderr, "out of memory\n");
                exit(EXIT_FAILURE);
            }
            strcpy(obj->data.symbol.value, value);
    //      put it onto the hash table
            hashpost(value, obj);
    // endif
    }
    // return object
    return obj;
}

char is_symbol(object *obj) {
    return obj->type == SYMBOL;
}

object *quote_symbol() {

    static object *_quote_symbol = NULL;

    if (_quote_symbol == NULL)
        _quote_symbol = make_symbol("quote");

    return _quote_symbol;
}

object *define_symbol() {

    static object *_define_symbol = NULL;

    if (_define_symbol == NULL)
        _define_symbol = make_symbol("define");

    return _define_symbol;
}

object *set_symbol() {

    static object *_set_symbol = NULL;

    if (_set_symbol == NULL)
        _set_symbol = make_symbol("set!");

    return _set_symbol;
}

object *ok_symbol() {

    static object *_ok_symbol = NULL;

    if (_ok_symbol == NULL)
        _ok_symbol = make_symbol("ok");

    return _ok_symbol;
}

object *if_symbol() {

    static object *_if_symbol = NULL;

    if (_if_symbol == NULL)
        _if_symbol = make_symbol("if");

    return _if_symbol;
}



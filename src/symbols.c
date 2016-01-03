/* symbols.c */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <stdbool.h>
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

bool is_symbol(object *obj) {
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

object *lambda_symbol() {
    static object *_lambda_symbol = NULL;
    if (_lambda_symbol == NULL)
        _lambda_symbol = make_symbol("lambda");
    return _lambda_symbol;
}

object *begin_symbol() {
    static object *_begin_symbol = NULL;
    if (_begin_symbol == NULL)
        _begin_symbol = make_symbol("begin");
    return _begin_symbol;
}

object *cond_symbol() {
    static object *_cond_symbol = NULL;
    if (_cond_symbol == NULL)
        _cond_symbol = make_symbol("cond");
    return _cond_symbol;
}

object *else_symbol() {
    static object *_else_symbol = NULL;
    if (_else_symbol == NULL)
        _else_symbol = make_symbol("else");
    return _else_symbol;
}

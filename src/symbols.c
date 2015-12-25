/* symbols.c */

#include "ptscheme.h"
#include "memmanager.h"
#include "hashtable.h"

object *make_symbol(char *value) {

    object *obj;
    namelist *element;

    /* search for they symbol in the symbol table and create it if it does not exist */

    element = hashput(value, "");

    /* allocate and return a new lisp object that points to the symbol */

    obj = alloc_object();
    obj->type = SYMBOL;
    obj->data.symbol.element = element;

    return obj;
}

char is_symbol(object *obj) {
    return obj->type == SYMBOL;
}

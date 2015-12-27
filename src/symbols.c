/* symbols.c */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "ptscheme.h"
#include "memmanager.h"
#include "hashtable.h"

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

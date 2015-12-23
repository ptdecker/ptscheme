/**
 * hashtable.c - A basic hash table implementation
 *
 * Based upon K&R section 6.6, pp. 143-145
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "hashtable.h"

/**
 * Create a hash key based upon a string
 * @param  s String upon which hash key should be based
 * @return   Unsigned integer hash key
 */
unsigned genhashkey(const char *s) {

    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;

    return hashval % HASHSIZE;

} // genhashkey()

/**
 * Look up a key-value pair from the hash table
 * @param  s value to find
 * @return   pointer to stored value (NULL == not found)
 */
namelist *hashget(const char *key) {

    namelist *np;

    for (np = hashtable[genhashkey(key)]; np != NULL; np = np->next)
        if (strcmp(key, np->key) == 0)
            return np;

    return NULL;
}

namelist *hashput(const char *key, const char *value) {

    namelist *np;
    unsigned hashval;

    if ((np = hashget(key)) == NULL) {

        // Key not found, entry not in hash table -- Add it

        np = (namelist *) malloc(sizeof(*np));
        if ((np == NULL) || ((np->key = strdup(key)) == NULL)) {
            fprintf(stderr, "Out of memory\n");
            exit(EXIT_FAILURE);
        }

        hashval = genhashkey(key);
        np->next = hashtable[hashval];
        hashtable[hashval] = np;

    } else {

        // Key found -- free previous value

        free((void *) np->value);
    }

    if ((np->value = strdup(value)) == NULL) {
            fprintf(stderr, "Out of memory\n");
            exit(EXIT_FAILURE);
    }

    return np;
}

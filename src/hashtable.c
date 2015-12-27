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

unsigned genhashkey(const char *s) {

    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;

    return hashval % HASHSIZE;

} // genhashkey()

object *hashget(const char *key) {

    namelist *np;

    for (np = hashtable[genhashkey(key)]; np != NULL; np = np->next)
        if (strcmp(key, np->key) == 0)
            return np->value;

    return NULL;
}

void hashpost(const char *key, object *value) {

    namelist *np;
    unsigned hashval;

    np = (namelist *)malloc(sizeof(*np));
    if (np == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }

    np->key = strdup(key);
    if (np->key == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit (EXIT_FAILURE);
    }

    np->value = value;

    hashval = genhashkey(key);
    np->next = hashtable[hashval];
    hashtable[hashval] = np;

    return;
}

void hashdump() {
    int i;
    namelist *np;
    printf("\nhash table\n");
    for (i = 0; i < HASHSIZE; i++)
        for (np = hashtable[i]; np != NULL; np = np->next)
            printf("\t%s\n", np->value->data.symbol.value);
}

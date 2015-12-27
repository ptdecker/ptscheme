/**
 * hashtable.h - A basic hash table implementation
 *
 * Based upon K&R section 6.6, pp. 143-145
 */

#ifndef FILE_HASHTABLE_SEEN
#define FILE_HASHTABLE_SEEN

#define HASHSIZE    101

#include "ptscheme.h"

/**
 * Single linked list of key-value pointer pairs
 */

typedef struct namelist {
    struct namelist *next;  // next list entry
    char   *key;   // key
    object *value; // value
} namelist;

/**
 * Hash table
 */

static namelist *hashtable[HASHSIZE];


unsigned genhashkey(const char *s);
extern object *hashget(const char *key);
extern void hashpost(const char *key, object *value);
extern void hashdump();

#endif // !FILE_HASHTABLE_SEEN

/**
 * hashtable.h - A basic hash table implementation
 *
 * Based upon K&R section 6.6, pp. 143-145
 */

#ifndef FILE_HASHTABLE_SEEN
#define FILE_HASHTABLE_SEEN

#define HASHSIZE    101

/**
 * Single linked list of key-value pointer pairs
 */

typedef struct namelist {
    struct namelist *next;  // next list entry
    char   *key;   // key
    char   *value; // value
} namelist;

/**
 * Hash table
 */

static namelist *hashtable[HASHSIZE];


unsigned genhashkey(const char *s);
extern namelist *get(const char *key);
extern namelist *put(const char *key, const char *value);

#endif // !FILE_HASHTABLE_SEEN

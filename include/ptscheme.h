/*
 * ptscheme.h
 */

#ifndef FILE_PTSCHEME_SEEN
#define FILE_PTSCHEME_SEEN

/* Const declarations */

#define BUFFER_MAX 1000

/* Type declarations */

typedef enum {
    BOOLEAN,
    CHARACTER,
    EMPTY_LIST,
    FIXNUM,
    PAIR,
    STRING,
    ERROR
} object_type;

typedef struct object {
    object_type type;
    union {
        struct {
            char value;
        } boolean;
        struct {
            char value;
        } character;
        struct {
            // Empty
        } empty;
        struct {
            long value;
        } fixnum;
        struct {
            struct object *car;
            struct object *cdr;
        } pair;
        struct {
            char *value;
        } string;
        struct {
            long error_num;
            char *error_msg;
        } error;
    } data;
} object;

#endif  // !FILE_PTSCHEME_SEEN

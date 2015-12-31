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
    PRIMITIVE_PROC,
    STRING,
    SYMBOL,
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
            struct object *(*fn)(struct object *arguments);
        } primitive_proc;
        struct {
            char *value;
        } string;
        struct {
            char *value;
        } symbol;
        struct {
            long error_num;
            char *error_msg;
        } error;
    } data;
} object;

/* primitives.h */

extern object *add_proc(object *arguments);

#endif  // !FILE_PTSCHEME_SEEN

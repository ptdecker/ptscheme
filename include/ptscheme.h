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
    COMPOUND_PROC,
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
            struct object *parameters;
            struct object *body;
            struct object *env;
        } compound_proc;
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

/* Lambda suport (lambda.h) */

#include <stdbool.h>

extern object *make_compound_proc(object *parameters, object *body, object* env);
extern bool is_compound_proc(object *obj);
extern object *make_lambda(object *parameters, object *body);
extern bool is_lambda(object *exp);
extern object *lambda_parameters(object *exp);
extern object *lambda_body(object *exp);

#endif  // !FILE_PTSCHEME_SEEN

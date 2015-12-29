/* replread.c */
/* REPL - Read */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ptscheme.h"
#include "lispbool.h"
#include "lisppair.h"
#include "hashtable.h"
#include "replprint.h"

/* REPL - Print */

void expand_esc_seq(char str[], const char c) {

    switch (c) {
        case '\0':  // Null
            strcat(str, "NULL");
            break;
        case '\a':  // Alarm (Beep, bell character)
            strcat(str, "\\a");
            break;
        case '\b':  // Backspace
            strcat(str, "\\b");
            break;
        case '\f':  // Formfeed
            strcat(str, "\\f");
            break;
        case '\n':  // Newline (Linefeed)
            strcat(str, "\\n");
            break;
        case '\r':  // Carriage return
            strcat(str, "\\r");
            break;
        case '\t':  // Tab
            strcat(str, "\\t");
            break;
        case '\v':  // Vertical tab
            strcat(str, "\\v");
            break;
        case '\\': // Backslash
            strcat(str, "\\\\");
            break;
        case '\'': // Single quote
            strcat(str, "\\'");
            break;
        case '\"':  // Double quote
            strcat(str, "\\\"");
            break;
        case '\?':  // Question mark
            strcat(str, "\\?");
            break;
        default:
            str[0] = c;
            str[1] = '\0';
    } // switch
    return;
} // expand_esc_seq

/* Resolve forward reference to 'write' */

void write(object *obj);

void write_pair(object *pair) {
    object *car_obj;
    object *cdr_obj;

    car_obj = car(pair);
    cdr_obj = cdr(pair);
    write(car_obj);
    if (cdr_obj->type == PAIR) {
        printf(" ");
        write_pair(cdr_obj);
    } else if (cdr_obj->type == EMPTY_LIST)
        return;
    else {
        printf(" . ");
        write(cdr_obj);
    }
}

void write(object *obj) {

    char *str  = NULL;
    char str2[5];

    switch (obj->type) {
        case BOOLEAN:
            printf("(BOOL) #%c", is_false(obj) ? 'f' : 't');
            break;
        case CHARACTER:
            str2[0] = '\0';
            expand_esc_seq(str2, obj->data.character.value);
            printf("(CHAR) #'%s'", str2);
            break;
        case EMPTY_LIST:
            printf("(ELIST) ()");
            break;
        case FIXNUM:
            printf("(FIXNUM) %ld", obj->data.fixnum.value);
            break;
        case PAIR:
            printf("(PAIR) (");
            write_pair(obj);
            printf(")");
            break;
        case STRING:
            str = obj->data.string.value;
            printf("(STR) \"");
//            putchar('"');
            while (*str != '\0') {
                str2[0] = '\0';
                expand_esc_seq(str2, *str);
                printf("%s", str2);
                str++;
            }
            putchar('"');
            break;
        case SYMBOL:
            printf("(SYM) %s", obj->data.symbol.value);
            break;
        case ERROR:
            printf("Error %ld: %s", obj->data.error.error_num, obj->data.error.error_msg);
            break;
        default:
            fprintf(stderr, "cannot write unknown type\n");
            exit(EXIT_FAILURE);
    } // switch

} // write()

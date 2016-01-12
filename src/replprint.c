/* replprint.c */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ptscheme.h"
#include "lispbool.h"
#include "lisppair.h"
#include "hashtable.h"
#include "replprint.h"

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

// TODO: The code below smells
void write_pair(FILE *out, object *pair) {
    object *car_obj;
    object *cdr_obj;

    car_obj = car(pair);
    cdr_obj = cdr(pair);
    write(out, car_obj);
    if (cdr_obj->type == PAIR) {
        fprintf(out, " ");
        write_pair(out, cdr_obj);
    } else if (cdr_obj->type == EMPTY_LIST)
        return;
    else {
        fprintf(out, " . ");
        write(out, cdr_obj);
    }
}

void write(FILE *out, object *obj) {

    char *str  = NULL;
    char str2[5];

    switch (obj->type) {
        case BOOLEAN:
            fprintf(out, "#%c", is_false(obj) ? 'f' : 't');
            break;
        case CHARACTER:
            str2[0] = '\0';
            expand_esc_seq(str2, obj->data.character.value);
            fprintf(out, "#'%s'", str2);
            break;
        case COMPOUND_PROC:
            fprintf(out, "#<compound-procedure>");
            break;
        case EMPTY_LIST:
            fprintf(out, "()");
            break;
        case EOF_OBJECT:
            fprintf(out, "#<eof>");
            break;
        case FIXNUM:
            fprintf(out, "%ld", obj->data.fixnum.value);
            break;
        case FLOATNUM:
            fprintf(out, "%f", obj->data.floatnum.value);
            break;
        case INPUT_PORT:
            fprintf(out, "#<input-port>");
            break;
        case OUTPUT_PORT:
            fprintf(out, "#<output-port>");
            break;
        case PAIR:
            fprintf(out, "(");
            write_pair(out, obj);
            fprintf(out, ")");
            break;
        case PRIMITIVE_PROC:
            fprintf(out, "#<primitive-procedure>");
            break;
        case STRING:
            str = obj->data.string.value;
            fputc('"', out);
            while (*str != '\0') {
                str2[0] = '\0';
                expand_esc_seq(str2, *str);
                fprintf(out, "%s", str2);
                str++;
            }
            fputc('"', out);
            break;
        case SYMBOL:
            fprintf(out, "%s", obj->data.symbol.value);
            break;
        case ERROR:
            fprintf(out, "Error %ld: %s", obj->data.error.error_num, obj->data.error.error_msg);
            break;
        default:
            fprintf(stderr, "cannot write unknown type\n");
            exit(EXIT_FAILURE);
    } // switch

} // write()

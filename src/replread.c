/* replread.c */
/* REPL - Read */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "ptscheme.h"
#include "memmanager.h"
#include "lispbool.h"
#include "lispchar.h"
#include "lispint.h"
#include "lispstr.h"
#include "lisperr.h"
#include "lisppair.h"
#include "symbols.h"
#include "replread.h"

bool is_delimiter(int c) {
    return (isspace(c) ||
            c == EOF   ||
            c == '('   ||
            c == ')'   ||
            c == '"'   ||
            c == ';');
}

bool is_initial(int c) {
    return (isalpha(c) ||
            c == '*'   ||
            c == '/'   ||
            c == '>'   ||
            c == '<'   ||
            c == '='   ||
            c == '?'   ||
            c == '!');
}

int peek(FILE *in) {
    int c;
    c = getc(in);
    ungetc(c, in);
    return c;
}

void eat_whitespace(FILE *in) {
    int c;
    while ((c = getc(in)) != EOF) {
        if (isspace(c))
            continue;
        if (c == ';') {
            while (((c = getc(in)) != EOF) && (c != '\n'));
            continue;
        }
        ungetc(c, in);
        break;
    }
}

void flush_input(FILE *in) {
    int c;
    while (((c = getc(in)) != EOF) && (c != '\n'));
}

char make_esc_seq(const char c) {
    switch (c) {
        case '0':  // Null
            return '\0';
        case 'a':  // Alarm (Beep, bell character)
            return '\a';
        case 'b':  // Backspace
            return '\b';
        case 'f':  // Formfeed
            return '\f';
        case 'n':  // Newline (Linefeed)
            return '\n';
        case 'r':  // Carriage return
            return '\r';
        case 't':  // Tab
            return '\t';
        case 'v':  // Vertical tab
            return '\v';
        case '\\': // Backslash
            return '\\';
        case '\'': // Single quote
            return '\'';
        case '"':  // Double quote
            return '\"';
        case '?':  // Question mark
            return '\?';
        default:
            return c;
    }
}

object *read_character(FILE *in) {

    // Coming in here, we have already read the sequence:  #'

    int c;
    int trash;

    c = getc(in);

    // Either EOF or end of line encountered early

    if ((c == EOF) || (c == '\n'))
        return make_error(4, "incomplete character literal");

    // Start of escape sequence encountered

    if (c == '\\') {

        c = getc(in);

        // Either EOF or end of line encountered early

        if ((c == EOF) || (c == '\n'))
            return make_error(4, "incomplete character literal");

        // Make sure next charactor is the terminiating single-quote

        if (peek(in) != '\'')
            return make_error(4, "character literal missing termination");

        c = make_esc_seq(c);

    }

    // Make sure next charactor is the terminiating single-quote

    if (peek(in) != '\'')
        return make_error(4, "character literal missing termination");

    trash = getc(in); // eat the postfix single quote
    return (make_character(c));

}

object *read_pair(FILE *in) {

    int c;
    object *car_obj;
    object *cdr_obj;

    // Tighten things up

    eat_whitespace(in);

    // Handle the empty list condition (where we immediately get our closed paren)

    if (peek(in) == ')') {
        c = getc(in); // Eat the closed paren
        return empty_list();
    }

    // Recursively read the car portion of the pair

    car_obj = read(in);

    // Tighten things up

    eat_whitespace(in);

    // Handle an improper list

    if (peek(in) == '.') {

        c = getc(in); // Eat the period

        // Check for a delimiter following the dot

        c = peek(in);
        if (!is_delimiter(c)) {
            flush_input(in);
            return make_error(34, "dot not followed by a delimiter");
        }

        // Recursively read the cdr portion of the pair

        cdr_obj = read(in);

        // Tighten up

        eat_whitespace(in);

        // Make sure we have our closed paren

        c = getc(in);
        if (c != ')') {
            flush_input(in);
            return make_error(35, "trailing right parenthesis is missing after dotted cdr");
        }

        // Then paste everything together

        return cons(car_obj, cdr_obj);
    }

    // Recursively read the cdr portion of the pair

    cdr_obj = read_pair(in);

    // Then pair them up with cons

    return cons(car_obj, cdr_obj);
}

object *read(FILE *in) {

    int   c;
    int   i;
    short sign = 1;
    long  num  = 0;
    char  buffer[BUFFER_MAX];

    eat_whitespace(in);

    c = getc(in);

    // Handle '#' (booleans, characters)

    if (c == '#') {

        if (peek(in) == EOF || peek(in) == '\n')
            return make_error(10, "unexpected end of line encountered");

        c = getc(in);
        switch (c) {
            case 't':
                return make_boolean(true);
            case 'f':
                return make_boolean(false);
            case '\'':
                return read_character(in);
            default:
                flush_input(in);
                return make_error(3, "unknown boolean literal");
        }
        // Should never reach here
    }

    // Handle digits and negation followed by a digit (integers)

    if (isdigit(c) || (c == '-' && (isdigit(peek(in))))) {

        if (c == '-')
            sign = -1;
        else
            ungetc(c, in);

        while (isdigit(c = getc(in)))
            num = (10 * num) + (c - '0');

        num *= sign;

        if (is_delimiter(c)) {
            ungetc(c, in);
            return make_fixnum(num);
        } else {
            flush_input(in);
            return make_error(1, "number not followed by delimiter");
        }

        // Should never reach here
        fprintf(stderr, "invalid state\n");
        exit(EXIT_FAILURE);

    }

    // Handle quote as an initial (strings)

    if (c == '"') {

        i = 0;

        while (true) {

            c = getc(in);

            // At EOF before string termination

            if (c == EOF) {
                flush_input(in);
                return make_error(6, "non-terminated string literal\n");
            }

            // String termination reached

            if (c == '\"')
                break;

            // Make sure we have enough space left in our buffer

            if (i == (BUFFER_MAX - 1))
                return make_error(7, "string too long");

            // Convert escape sequences into control characters

            if (c == '\\') {
                c = getc(in);
                if (c == EOF) {
                    flush_input(in);
                    return make_error(7, "incomplete  literal");
                }
                c = make_esc_seq(c);
            }

            buffer[i++] = c;

        } // while getting characters

        buffer[i] = '\0';
        return make_string(buffer);

        // Should never reach here

    }

    // Handle open parentesis (lists)

    if (c == '(')
        return read_pair(in);

    // Handle symbols

    if (is_initial(c) || ((c == '+' || c == '-') && is_delimiter(peek(in)))) {

        i = 0;

        while (is_initial(c) || isdigit(c) || c == '+' || c == '-') {

            if (i < BUFFER_MAX - 1)
                buffer[i++] = c;
            else
                return make_error(40, "symbol too long. ");

            c = getc(in);
        }

        if (is_delimiter(c)) {
            buffer[i] = '\0';
            ungetc(c, in);
            return make_symbol(buffer);
        }

        return make_error(41, "symbol not followed by a delimiter");

    }

    if (c == '\'') {
        return cons(quote_symbol(), cons(read(in), empty_list()));
    }

    if (c == EOF) {
        return NULL;
    }

    // We should have handled everything by now, if not then illegal state

    flush_input(in);
    fprintf(stderr, "read illegal state\n");
    exit(EXIT_FAILURE);
}

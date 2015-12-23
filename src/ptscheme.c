/*
 * A basic implementation of Scheme in 'C'
 *
 * LICENSE INFO
 * ------------
 *
 * The licensed libraries used by this project are:
 *    - Brian Fox's GNU Readline under GPL v3 (now maintained by Chet Ramey) 
 *           c.f. http://cnswww.cns.cwru.edu/php/chet/readline/rltop.html
 *
 * This applicaiton is based partially upon the shoulders of:
 *    - Daniel Holden's MPC under BSD3
 *           c.f. http://buildyourownlisp.com/
 *           c.f. https://github.com/orangeduck/BuildYourOwnLisp
 *    - Peter Michaux's "Scheme from Scratch"
 *           c.f. http://peter.michaux.ca/articles/scheme-from-scratch-introduction
 *           c.f. https://github.com/petermichaux/bootstrap-scheme
 *			- Changes from the Michaux version:
 *				- Inclusion of an error type so that malformed input doesn't kill the app
 *              - Handled the singleton boolean object type differently to parallel other objects
 *              - Implemented characters using C-style character literals
 *				- Included support for all C-style escape sequences except for octal and hex
 *				- Echoed the way the singleton boolean was handled for singleton empty
 *
 * Due to the inclusion of GNU Readline, this project is also licensed under GPL v3
 */

/* Define directives */

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

//TODO: Move empty list out into its own module once more list stuff is in here

/* Empty List */

object *make_empty() {

	// The empty list is a singletons

	static object *empty_list;

	if (empty_list == NULL) {
		empty_list = alloc_object();
		empty_list->type = EMPTY_LIST;
	}

	return empty_list;
}

bool is_empty(object *obj) {
	return (obj->type == EMPTY_LIST);
}

/* Pair */

object *cons(object *car, object *cdr) {

    object *obj;
    
    obj = alloc_object();
    obj->type = PAIR;
    obj->data.pair.car = car;
    obj->data.pair.cdr = cdr;

    return obj;
}

char is_pair(object *obj) {
    return obj->type == PAIR;
}

object *car(object *pair) {

	if (!is_pair(pair)) {
		fprintf(stderr, "%p (type: %d) is not a pair\n", pair, pair->type);
    	exit(EXIT_FAILURE);
	}

    return pair->data.pair.car;
}

void set_car(object *obj, object* value) {
    obj->data.pair.car = value;
}

object *cdr(object *pair) {
	
	if (!is_pair(pair)) {
		fprintf(stderr, "%p (type: %d) is not a pair\n", pair, pair->type);
    	exit(EXIT_FAILURE);
	}

    return pair->data.pair.cdr;
}

void set_cdr(object *obj, object* value) {
    obj->data.pair.cdr = value;
}

#define caar(obj)   car(car(obj))
#define cadr(obj)   car(cdr(obj))
#define cdar(obj)   cdr(car(obj))
#define cddr(obj)   cdr(cdr(obj))
#define caaar(obj)  car(car(car(obj)))
#define caadr(obj)  car(car(cdr(obj)))
#define cadar(obj)  car(cdr(car(obj)))
#define caddr(obj)  car(cdr(cdr(obj)))
#define cdaar(obj)  cdr(car(car(obj)))
#define cdadr(obj)  cdr(car(cdr(obj)))
#define cddar(obj)  cdr(cdr(car(obj)))
#define cdddr(obj)  cdr(cdr(cdr(obj)))
#define caaaar(obj) car(car(car(car(obj))))
#define caaadr(obj) car(car(car(cdr(obj))))
#define caadar(obj) car(car(cdr(car(obj))))
#define caaddr(obj) car(car(cdr(cdr(obj))))
#define cadaar(obj) car(cdr(car(car(obj))))
#define cadadr(obj) car(cdr(car(cdr(obj))))
#define caddar(obj) car(cdr(cdr(car(obj))))
#define cadddr(obj) car(cdr(cdr(cdr(obj))))
#define cdaaar(obj) cdr(car(car(car(obj))))
#define cdaadr(obj) cdr(car(car(cdr(obj))))
#define cdadar(obj) cdr(car(cdr(car(obj))))
#define cdaddr(obj) cdr(car(cdr(cdr(obj))))
#define cddaar(obj) cdr(cdr(car(car(obj))))
#define cddadr(obj) cdr(cdr(car(cdr(obj))))
#define cdddar(obj) cdr(cdr(cdr(car(obj))))
#define cddddr(obj) cdr(cdr(cdr(cdr(obj))))

/* REPL - Read */

bool is_delimiter(int c) {
	return (isspace(c) ||
			c == EOF   ||
			c == '('   ||
			c == ')'   ||
			c == '"'   ||
			c == ';'); 
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
/*
//TODO:  Get rid of the hard exit in eat_expected_string

void eat_expected_string(FILE *in, char *str) {
    int c;

    while (*str != '\0') {
        c = getc(in);
        if (c != *str) {
            fprintf(stderr, "unexpected character '%c'\n", c);
            exit(1);
        }
        str++;
    }
}

//TODO:  Get rid of the hard exit in peek_expected_delimiter

void peek_expected_delimiter(FILE *in) {
    if (!is_delimiter(peek(in))) {
        fprintf(stderr, "character not followed by delimiter\n");
        exit(1);
    }
}
*/
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

	if ((c == EOF) || (c == '\n')) {
		return make_error(4, "incomplete character literal");
	}

	// Start of escape sequence encountered

	if (c == '\\') {

		c = getc(in);

		// Either EOF or end of line encountered early

		if ((c == EOF) || (c == '\n')) {
			return make_error(4, "incomplete character literal");
		}

		// Make sure next charactor is the terminiating single-quote

		if (peek(in) != '\'') {
			return make_error(4, "character literal missing termination");
		}

		c = make_esc_seq(c);

	}

	// Make sure next charactor is the terminiating single-quote

	if (peek(in) != '\'') {
		return make_error(4, "character literal missing termination");
	}

	trash = getc(in); // eat the postfix single quote
	return (make_character(c));

}

/* Define forward reference */

object *read(FILE *in);

object *read_pair(FILE *in) {

    int c;
    object *car_obj;
    object *cdr_obj;
    
    // Tighten things up

    eat_whitespace(in);

    // Handle the empty list condition (where we immediately get our closed paren)

    if (peek(in) == ')') {
    	c = getc(in); // Eat the closed paren
    	return make_empty();
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

	if (c == '#') {

		if (peek(in) == EOF || peek(in) == '\n') {
			return make_error(10, "unexpected end of line encountered");
		}

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

	} else if (isdigit(c) || (c == '-' && (isdigit(peek(in))))) {

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

	} else if (c == '"') {

		i = 0;

        while (true) {

        	c = getc(in);

        	// At EOF before string termination

            if (c == EOF) {
            	flush_input(in);
            	return make_error(6, "non-terminated string literal\n");
            }

            // String termination reached

        	if (c == '\"') {
        		break;
        	}

            // Make sure we have enough space left in our buffer

            if (i == (BUFFER_MAX - 1)) {
            	return make_error(7, "string too long");
            }

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

    } else if (c == '(') {

    	return read_pair(in);

	} else {
		flush_input(in);
		return make_error(2, "bad input");
	}

	fprintf(stderr, "read illegal state\n");
	exit(EXIT_FAILURE);
}

/* REPL - Evaluate */

object *eval(object *exp) {
	return exp;
}

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
    } else if (cdr_obj->type == EMPTY_LIST) {
        return;
    } else {
        printf(" . ");
        write(cdr_obj);
    }
}

void write(object *obj) {

	char *str  = NULL;
	char str2[5];

	switch (obj->type) {
		case BOOLEAN:
			printf("#%c", is_false(obj) ? 'f' : 't');
			break;
		case CHARACTER:
			str2[0] = '\0';
			expand_esc_seq(str2, obj->data.character.value);
			printf("#'%s'", str2);
			break;
		case EMPTY_LIST:
			printf("()");
			break;
		case FIXNUM:
			printf("%ld", obj->data.fixnum.value);
			break;
		case PAIR:
			printf("(");
			write_pair(obj);
			printf(")");
			break;
		case STRING:
		    str = obj->data.string.value;
            putchar('"');
            while (*str != '\0') {
            	str2[0] = '\0';
            	expand_esc_seq(str2, *str);
            	printf("%s", str2);
                str++;
            }
            putchar('"');
            break;
		case ERROR:
			printf("Error %ld: %s", obj->data.error.error_num, obj->data.error.error_msg);
			break;
		default:
			fprintf(stderr, "cannot write unknown type\n");
			exit(EXIT_FAILURE);
	} // switch

} // write()

/* REPL */

int main(void) {
	printf("ptscheme v0.0.1\n");
	printf("Ctrl-c to exit\n\n");
	while(true) {
		printf("> ");
		write(eval(read(stdin)));
		printf("\n");
	}
	return EXIT_SUCCESS;
}
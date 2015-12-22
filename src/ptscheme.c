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

	flush_input(in); // Dump the rest of the line
	return (make_character(c));

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

/* REPL - Print */

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
		case FIXNUM:
			printf("%ld", obj->data.fixnum.value);
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
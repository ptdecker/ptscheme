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

/* Const declarations */

const int BUFFER_MAX = 1000;

/* Type declarations */

typedef enum {
	BOOLEAN,
	CHARACTER,
	FIXNUM,
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
			long value;
		} fixnum;
		struct {
			char *value;
		} string;
		struct {
			long error_num;
			char *error_msg;
		} error;
	} data;
} object;

/* Lisp object constructors */

object *alloc_object(void) {
	object *obj;
	obj = malloc(sizeof(object));
	if (obj == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}
	return obj;
}

// Boolean native type

object *make_boolean(char value) {

	// The boolean objects are singletons

	static object *bool_true;
	static object *bool_false;

	if (bool_true == NULL) {
		bool_true = alloc_object();
		bool_true->type = BOOLEAN;
		bool_true->data.boolean.value = true;
	}
	if (bool_false == NULL) {
		bool_false = alloc_object();
		bool_false->type = BOOLEAN;
		bool_false->data.boolean.value = false;
	}
	return ((value) ? bool_true : bool_false);
}

bool is_boolean(object *obj) {
	return (obj->type == BOOLEAN);
}

bool is_false(object *obj) {
	return (obj->data.boolean.value == false);
}

bool is_true(object *obj) {
	return (obj->data.boolean.value == true);
}

// Character native type

object *make_character(char value) {
	object *obj;
	obj = alloc_object();
	obj->type = CHARACTER;
	obj->data.character.value = value;
	return obj;
}

bool is_character(object *obj) {
	return (obj->type == CHARACTER);
}

// Integer native type

object *make_fixnum(long value) {
	object *obj;
	obj = alloc_object();
	obj->type = FIXNUM;
	obj->data.fixnum.value = value;
	return obj;
}

bool is_fixnum(object *obj) {
	return (obj->type == FIXNUM);
}

// String native type

object *make_string(char *value) {
	object *obj;
	obj = alloc_object();
	obj->type = STRING;
	obj->data.string.value = malloc(strlen(value) + 1);
	if (obj->data.string.value == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(obj->data.string.value, value);
	return obj;
}

bool is_string(object *obj) {
	return (obj->type == STRING);
}

// Error native type

object *make_error(long error_num, char *error_msg) {
	object *obj;
	obj = alloc_object();
	obj->type = ERROR;
	obj->data.error.error_num = error_num;
	obj->data.error.error_msg = error_msg;
	return obj;
}

bool is_error(object *obj) {
	return (obj->type == ERROR);
}

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
		else if (c == ';') {
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

object *read_character(FILE *in) {

	int c;
	int n; // next character (use to check for postfix single quote)

	c = getc(in);

	switch (c) {
		case EOF:
			return make_error(4, "incomplete character literal");
		case '\n':
			return make_error(4, "incomplete character literal");
		case '\'':
			c = '\000';
			break;
		case '\\':
			c = getc(in);
			switch (c) {
				case EOF:
				case '\n':
					flush_input(in);
					return make_error(4, "incomplete character literal");
				case 'a':  // Alarm (Beep, bell character)
					c = '\a';
					break;
				case 'b':  // Backspace
					c = '\b';
					break;
				case 'f':  // Formfeed
					c = '\f';
					break;
				case 'n':  // Newline (Linefeed)
					c = '\n';
					break;
				case 'r':  // Carriage return
					c = '\r';
					break;
				case 't':  // Tab
					c = '\t';
					break;
				case 'v':  // Vertical tab
					c = '\v';
					break;
				case '\\': // Backslash
					c = '\\';
					break;
				case '\'': // Single quote
					if (peek(in) == '\'') {
						c = '\'';
						break;
					} else {
						flush_input(in);
						return make_error(4, "incomplete character literal--missing terminating single quote");
					}
				case '"':  // Double quote
					c = '\"';
					break;
				case '?':  // Question mark
					c = '\?';
					break;
				default:
					flush_input(in);
					return make_error(6, "invalid character literal escape code");
			}
	}

	if (c) {
		n = getc(in);
		if (n != '\'') {
			flush_input(in);
			return make_error(4, "incomplete character literal--missing terminating single quote");
		}
	}

	if (!is_delimiter(peek(in))) {
		return make_error(5, "character literal not followed by delimiter");
	}

	return (make_character(c));

}

object *read(FILE *in) {

	int   c;
	int   i;
	short sign = 1;
	long  num  = 0;
	char  buffer[BUFFER_MAX];
	bool  closed;

	eat_whitespace(in);

	c = getc(in);

	if (c == '#') {

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
		closed = false;

        while (!closed) {

        	c = getc(in);

            if (c == EOF) {
            	flush_input(in);
            	return make_error(6, "non-terminated string literal\n");
            }

        	if (c == '\"') {
        		closed = true;
        		continue;
        	}

            switch (c) {
            	case '\a':
            		c = '\a';
            		break;
            	case '\b':
            		c = '\b';
            		break;
            	case '\f':
            		c = '\f';
            		break;
            	case '\n':
            		c = '\n';
            		break;
            	case '\r':
            		c = '\r';
            		break;
            	case '\t':
            		c = '\t';
            		break;
            	case '\v':
            		c = '\v';
            		break;
            	case '\\':
                	c = getc(in);
					switch (c) {
						case EOF:
							flush_input(in);
							return make_error(7, "incomplete  literal");
						case 'a':  // Alarm (Beep, bell character)
							c = '\a';
							break;
						case 'b':  // Backspace
							c = '\b';
							break;
						case 'f':  // Formfeed
							c = '\f';
							break;
						case 'n':  // Newline (Linefeed)
							c = '\n';
							break;
						case 'r':  // Carriage return
							c = '\r';
							break;
						case 't':  // Tab
							c = '\t';
							break;
						case 'v':  // Vertical tab
							c = '\v';
							break;
						case '\\': // Backslash
							c = '\\';
							break;
						case '\'': // Single quote
							c = '\'';
							break;
						case '"':  // Double quote
							c = '\"';
							break;
						case '?':  // Question mark
							c = '\?';
							break;
					}
                	break;
                case '\'':
                	c = '\'';
                	break;
                case '\?':
                	c = '\?';
                	break;

        	} // switch

            if (i < BUFFER_MAX - 1) {
                buffer[i++] = c;
            } else {
            	flush_input(in);
            	return make_error(7, "string too long");
            }

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

/* REPL - Print */

void write(object *obj) {

	char c;
	char *str;

	switch (obj->type) {
		case BOOLEAN:
			printf("#%c", is_false(obj) ? 'f' : 't');
			break;
		case CHARACTER:
			c = obj->data.character.value;
			printf("#'");
			switch (c) {
				case '\000':  // Null
					printf("NULL");
					break;
				case '\a':  // Alarm (Beep, bell character)
					printf("\\a");
					break;
				case '\b':  // Backspace
					printf("\\b");
					break;
				case '\f':  // Formfeed
					printf("\\f");
					break;
				case '\n':  // Newline (Linefeed)
					printf("\\n");
					break;
				case '\r':  // Carriage return
					printf("\\r");
					break;
				case '\t':  // Tab
					printf("\\t");
					break;
				case '\v':  // Vertical tab
					printf("\\v");
					break;
				case '\\': // Backslash
					printf("\\\\");
					break;
				case '\'': // Single quote
					printf("\\'");
					break;
				case '\"':  // Double quote
					printf("\\\"");
					break;
				case '\?':  // Question mark
					printf("\\?");
					break;
				default:
					putchar(c);
			}
			printf("'");
			break;
		case FIXNUM:
			printf("%ld", obj->data.fixnum.value);
			break;
		case STRING:
		    str = obj->data.string.value;
            putchar('"');
            while (*str != '\0') {
                switch (*str) {
					case '\a':
						printf("\\a");
						break;
					case '\b':
						printf("\\b");
						break;
					case '\f':
						printf("\\f");
						break;
                    case '\n':
                        printf("\\n");
                        break;
					case '\r': 
						printf("\\r");
						break;
					case '\t':
						printf("\\t");
						break;
					case '\v':
						printf("\\v");
						break;
                    case '\\':
                        printf("\\\\");
                        break;
					case '\'':
						printf("\\'");
						break;
					case '\"':
						printf("\\\"");
						break;
					case '\?':
						printf("\\?");
						break;
                    default:
                        putchar(*str);
                }
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
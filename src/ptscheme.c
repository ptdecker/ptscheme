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
 *           c.f. http://http://peter.michaux.ca/articles/scheme-from-scratch-introduction
 *           c.f. https://github.com/petermichaux/bootstrap-scheme
 *
 * Due to the inclusion of GNU Readline, this project is also licensed under GPL v3
 */

/* Define directives */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/* Type declarations */

typedef enum {
	FIXNUM,
	ERROR
} object_type;

typedef struct object {
	object_type type;
	union {
		struct {
			long value;
		} fixnum;
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

object *read(FILE *in) {

	int   c;
	short sign = 1;
	long  num  = 0;

	eat_whitespace(in);

	c = getc(in);

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
	switch (obj->type) {
		case FIXNUM:
			printf("%ld", obj->data.fixnum.value);
			break;
		case ERROR:
			printf("Error %ld: %s\n", obj->data.error.error_num, obj->data.error.error_msg);
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
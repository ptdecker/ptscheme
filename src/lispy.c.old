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
 *           c.f. https://github.com/orangeduck/mpc
 *
 * Due to the inclusion of GNU Readline, this project is also licensed under GPL v3
 */

/* Define directives */

//#define _WIN32  // Uncomment if compiling under Windoze

/* Define macros */

#define LASSERT(args, cond, fmt, ...) \
	if (!(cond)) { \
		lval* err = lval_err(fmt, ##__VA_ARGS__); \
		lval_del(args); \
		return err; \
	}

#define LASSERT_TYPE(func, args, index, expect) \
	LASSERT(args, args->cell[index]->type == expect, \
		"Function '%s' passed incorrect type for argument %i. Got %s, Expected %s.", \
		func, index, ltype_name(args->cell[index]->type), ltype_name(expect))

#define LASSERT_NUM(func, args, num) \
	LASSERT(args, args->count == num, \
		"Function '%s' passed incorrect number of arguments. Got %i, Expected %i.", \
		func, args->count, num)

#define LASSERT_NOT_EMPTY(func, args, index) \
	LASSERT(args, args->cell[index]->count != 0, \
		"Function '%s' passed {} for argument %i.", func, index);
    

/* Requires sudo apt-get install libedit-dev */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

/* Accomidate compiling on Windows which doesn't have editline available */

#ifdef _WIN32

	#include <string.h>

	static char buffer[2048];

	/* Fake readline function */

	char* readline(char* prompt) {
  		fputs(prompt, stdout);
  		fgets(buffer, 2048, stdin);
  		char* cpy = malloc(strlen(buffer)+1);
  		strcpy(cpy, buffer);
  		cpy[strlen(cpy)-1] = '\0';
  		return cpy;
	}

	/* Fake add_history function */

	void add_history(char* unused) {}

#else

	/* Otherwise include the editline headers */

	#include <editline/readline.h>
//	#include <histedit.h>

#endif

/* Handle all needed forward declarations */

struct lval;
struct lenv;

typedef struct lval lval;
typedef struct lenv lenv;

void  lval_print(lval* v);
lval* lval_pop(lval* v, int i);
lval* lval_take(lval* v, int i);
lval* lval_eval(lenv* e, lval* v);
lval* lval_join(lval* x, lval* y);
void  lenv_del(lenv* e);
lenv* lenv_copy(lenv* e);
lval* builtin(lenv* e, lval* a, char* func);
lval* builtin_op(lenv* e, lval* a, char* op);
lval* builtin_eval(lenv* e, lval* a);
lval* builtin_list(lenv* e, lval* a);

/* Declare lbuiltin function pointer */

typedef lval*(*lbuiltin)(lenv*, lval*);

/* Declare lval structure to carry errors and support multiple types */

struct lval {
	int      type;        // lisp value type

	/* Basic attributes */

	long     num;         // numeric value
	char*    err;         // error string
	char*    sym;         // symbol sting

	/* Function attributes */

	lbuiltin builtin;     // built-in function pointer
	lenv*    env;         // environment to store arguments
	lval*    formals;     // formal arguments
	lval*    body;        // Q-expression body of arguments

	/* Expression attributes */

	int      count;       // number of symbols
	struct   lval** cell; // self-referential pointer
};

/* Declare enviornment structure to hold defined variables */

struct lenv {
	lenv*	par;
	int 	count;
	char**	syms;
	lval**	vals;
};

/* Create enumerated types for supported lisp value types */

enum lval_types {
	LVAL_ERR,
	LVAL_NUM,
	LVAL_BOOL,
	LVAL_SYM,
	LVAL_FUN,
	LVAL_SEXPR,
	LVAL_QEXPR
};

/* Construct a pointer to a new number type lisp value */

lval* lval_num(long x) {
	lval* v = malloc(sizeof(lval));
	v->type = LVAL_NUM;
	v->num  = x;
	return v;
}

/* Construct a pointer to a new boolean type lisp value */

lval* lval_bool(bool x) {
	lval* v = malloc(sizeof(lval));
	v->type = LVAL_BOOL;
	v->num  = (x) ? 1 : 0;
	return v;
}

/* Construct a pointer to a new error type lisp value */

lval* lval_err(char* fmt, ...) {

	lval* v = malloc(sizeof(lval));
	v->type = LVAL_ERR;

	/* Create an initialize a va list */

	va_list va;
	va_start(va, fmt);

	/* Allocate 512 bytes of space for our error message buffer */

	v->err = malloc(512);

	/* printf into the error string with the maximum of 511 characters */

	vsnprintf(v->err, 511, fmt, va);

	/* reallocated the buffer space to the amount really used */

	v->err = realloc(v->err, strlen(v->err)+1);

	/* And, clean up */

	va_end(va);

	return v;

}

/* Construct a pointer to a new symbol type lisp value */

lval* lval_sym(char* s) {
	lval* v = malloc(sizeof(lval));
	v->type = LVAL_SYM;
	v->sym  = malloc(strlen(s) + 1);
	strcpy(v->sym, s);
	return v;
}

/* Construct a pointer to a new empty s-expression lisp value */

lval* lval_sexpr(void) {
	lval* v  = malloc(sizeof(lval));
	v->type  = LVAL_SEXPR;
	v->count = 0;
	v->cell  = NULL;
	return v;
}

/* Construct a pointer to a new empty q-expression lisp value */

lval* lval_qexpr(void) {
	lval* v  = malloc(sizeof(lval));
	v->type  = LVAL_QEXPR;
	v->count = 0;
	v->cell  = NULL;
	return v;
}

/* Construct a built-in function */

lval* lval_fun(lbuiltin func) {
	lval* v    = malloc(sizeof(lval));
	v->type    = LVAL_FUN;
	v->builtin = func;
	return v;
}

/* Construct an environment element */

lenv* lenv_new(void) {
	lenv* e = malloc(sizeof(lenv));
	e->par   = NULL;
	e->count = 0;
	e->syms  = NULL;
	e->vals  = NULL;
	return e;
}

/* Construct a lamda expression */

lval* lval_lambda(lval* formals, lval* body) {
	lval* v = malloc(sizeof(lval));
	v->type    = LVAL_FUN;
	v->builtin = NULL;
	v->env     = lenv_new();
	v->formals = formals;
	v->body    = body;
	return v;
}

/* Destruct a lisp value */

void lval_del(lval* v) {
	switch (v->type) {
		case LVAL_FUN:
			if (!v->builtin) {
				lenv_del(v->env);
				lval_del(v->formals);
				lval_del(v->body);
			}
			break;
		case LVAL_NUM:
		case LVAL_BOOL:
			// nothing to free up
			break;
		case LVAL_ERR:
			free(v->err);
			break;
		case LVAL_SYM:
			free(v->sym);
			break;
		case LVAL_SEXPR:
		case LVAL_QEXPR:
			for (int i = 0; i < v->count; i++) {
				lval_del(v->cell[i]);
			}
			free(v->cell);
			break;
	}
	free(v);
}

/* Destruct an environment value */

void lenv_del(lenv* e) {
	for (int i = 0; i < e->count; i++) {
		free(e->syms[i]);
		lval_del(e->vals[i]);
	}
	free(e->syms);
	free(e->vals);
	free(e);
}

/* Return a nice name for an argument type */

char* ltype_name(int t) {
	switch(t) {
		case LVAL_FUN:
			return "Function";
		case LVAL_NUM:
			return "Number";
		case LVAL_BOOL:
			return "Boolean";
		case LVAL_ERR:
			return "Error";
		case LVAL_SYM:
			return "Symbol";
		case LVAL_SEXPR:
			return "S-Expression";
		case LVAL_QEXPR:
			return "Q-Expression";
		default:
			return "Unknown";
	}
}

/* Add a lisp value to an s-expression */

lval* lval_add(lval* v, lval* x) {
	v->count++;
	v->cell = realloc(v->cell, sizeof(lval*) * v->count);
	v->cell[v->count-1] = x;
	return v;
}

/* Read a number lisp value */

//lval* lval_read_num(mpc_ast_t* t) {
//	errno = 0;
//	long x = strtol(t->contents, NULL, 10);
//	if (errno != ERANGE) {
//		return lval_num(x);
//	} else {
//		return lval_err("Error: Invalid number");
//	}
//}

/* Read an expression */

//lval* lval_read(mpc_ast_t* t) {

	/* If a symbol or a number, return conversion to that type */

//	if (strstr(t->tag, "number")) {
//		return lval_read_num(t);
//	}

//	if (strstr(t->tag, "boolean")) {
//		return lval_bool(strcmp(t->contents, "true") == 0);
//	}

//	if (strstr(t->tag, "symbol")) {
//		return lval_sym(t->contents);
//	}

	/* If the root or an s-expression, then create an empty list */

//	lval* x = NULL;

//	if ((strcmp(t->tag, ">") == 0) || (strstr(t->tag, "sexpr"))) {
//		x = lval_sexpr();
//	}

//	if (strstr(t->tag, "qexpr")) {
//		x = lval_qexpr();
//	}

	/* Fill this empty list with any valid expresssions contained wihtin */

//	for (int i = 0; i < t->children_num; i++) {
//		if (strcmp(t->children[i]->contents, "(") == 0) { continue; }
//		if (strcmp(t->children[i]->contents, ")") == 0) { continue; }
//		if (strcmp(t->children[i]->contents, "{") == 0) { continue; }
//		if (strcmp(t->children[i]->contents, "}") == 0) { continue; }
//		if (strcmp(t->children[i]->tag,  "regex") == 0) { continue; }
//		x = lval_add(x, lval_read(t->children[i]));
//	}

//	return x;

//}

/* Copy an lval */

lval* lval_copy(lval* v) {
  
	lval* x = malloc(sizeof(lval));
	x->type = v->type;
  
  	switch (v->type) {
    
    	/* Copy Functions and Numbers Directly */

    	case LVAL_FUN:
    		if (v->builtin) {
	    		x->builtin = v->builtin;
	    	} else {
	    		x->builtin = NULL;
	    		x->env     = lenv_copy(v->env);
	    		x->formals = lval_copy(v->formals);
	    		x->body    = lval_copy(v->body);
	    	}
    		break;
    	case LVAL_NUM:
    	case LVAL_BOOL:
    		x->num = v->num;
    		break;
    
	    /* Copy Strings using malloc and strcpy */

	    case LVAL_ERR:
	    	x->err = malloc(strlen(v->err) + 1);
	    	strcpy(x->err, v->err);
	    	break;
 		case LVAL_SYM:
 			x->sym = malloc(strlen(v->sym) + 1);
 			strcpy(x->sym, v->sym);
 			break;

	    /* Copy Lists by copying each sub-expression */

	    case LVAL_SEXPR:
    	case LVAL_QEXPR:
      		x->count = v->count;
      		x->cell = malloc(sizeof(lval*) * x->count);
      		for (int i = 0; i < x->count; i++) {
        		x->cell[i] = lval_copy(v->cell[i]);
      		}
    		break;
  	}

  	return x;

}

/* Print a lisp value expression */

void lval_expr_print(lval* v, char open, char close) {
	putchar(open);
	for (int i = 0; i < v->count; i++) {
		lval_print(v->cell[i]);
		if (i != (v->count - 1)) {
			putchar(' ');
		}
	}
	putchar(close);
}

/* Print a list value */

void lval_print(lval* v) {
	switch (v->type) {
		case LVAL_NUM:
			printf("%li", v->num);
			break;
		case LVAL_BOOL:
			printf("%s", (v->num) ? "true" : "false");
			break;
		case LVAL_ERR:
			printf("Error: %s", v->err);
			break;
		case LVAL_SYM:
			printf("%s", v->sym);
			break;
		case LVAL_SEXPR:
			lval_expr_print(v, '(', ')');
			break;
		case LVAL_QEXPR:
			lval_expr_print(v, '{', '}');
			break;
		case LVAL_FUN:
			if (v->builtin) {
				printf("<built-in function '%s'>", v->sym);
			} else {
				printf("(\\ ");
				lval_print(v->formals);
				putchar(' ');
				lval_print(v->body);
				putchar(')');
			}
			break;
	}
}

/* Print an lisp value followed by a new line */

void lval_println(lval* v) {
	lval_print(v);
	putchar('\n');
}

/* Retrieve an environment value */

lval* lenv_get(lenv* e, lval* k) {

	/* Iterate over all items in environment returning a copy of the value if
	 * the stored string matches; otherwise, return an error.
	 */

	for (int i = 0; i < e->count; i++) {
	    if (strcmp(e->syms[i], k->sym) == 0) {
	    	return lval_copy(e->vals[i]);
	    }
	}

	/* If no symbol then recursively check the parent environments; otherwise, error */

	if (e->par) {
		return lenv_get(e->par, k);
	} else {
		return lval_err("unbound symbol '%s'!", k->sym);
	}

}

/* Add a variable to an environment */

void lenv_put(lenv* e, lval* k, lval* v) {

	/* Iterate over all items in environment to see if variable already exists
	 * and if a variable is found, delete the item in that position replacing it
     * with the variable supplied by the user.
   	 */

   	for (int i = 0; i < e->count; i++) {
    	if (strcmp(e->syms[i], k->sym) == 0) {
      		lval_del(e->vals[i]);
      		e->vals[i] = lval_copy(v);
      		return;
    	}
  	}

	/* If no existing entry found then allocate space for new entry */

	e->count++;
	e->vals  = realloc(e->vals,  sizeof(lval*) * e->count);
	e->syms  = realloc(e->syms,  sizeof(char*) * e->count);

	/* Copy contents of lval and symbol string into new location */

	e->vals[e->count-1]  = lval_copy(v);
	e->syms[e->count-1]  = malloc(strlen(k->sym)+1);
	strcpy(e->syms[e->count-1], k->sym);

}

/* Add a variable to the root (i.e. top or global) environment */

void lenv_def(lenv* e, lval* k, lval* v) {

	/* Walk up the environment hiearchy to the top parent */

	while (e->par) {
		e = e->par;
	}

	lenv_put(e, k, v);
}

/* Copy an environment */

lenv* lenv_copy(lenv* e) {
	lenv* n = malloc(sizeof(lenv));
	n->par = e->par;
	n->count = e->count;
	n->syms = malloc(sizeof(char*) * n->count);
	n->vals = malloc(sizeof(lval*) * n->count);
	for (int i = 0; i < e->count; i++) {
		n->syms[i] = malloc(strlen(e->syms[i]) + 1);
		strcpy(n->syms[i], e->syms[i]);
		n->vals[i] = lval_copy(e->vals[i]);
	}
	return n;
}

/* Call a function */

lval* lval_call(lenv* e, lval* f, lval* a) {

	/* If Builtin then simply apply that */

	if (f->builtin) {
		return f->builtin(e, a);
	}

	/* Record Argument Counts */

	int given = a->count;
	int total = f->formals->count;

	/* While arguments still remain to be processed */

	while (a->count) {

		/* If we've ran out of formal arguments to bind */

		if (f->formals->count == 0) {
			lval_del(a);
			return lval_err("Function passed too many arguments. Got %i, Expected %i.", given, total);
		}

		/* Pop the first symbol from the formals */

		lval* sym = lval_pop(f->formals, 0);

		/* Special case to deal with the '&' symbol */

		if (strcmp(sym->sym, "&") == 0) {

			/* Ensure '&' is followed by another symbol */

			if (f->formals->count != 1) {
				lval_del(a);
				return lval_err("Function format invalid. Symbol '&' not followed by another symbol.");
			}

			/* Next formal should be bound to the remaining arguments */

			lval* nsym = lval_pop(f->formals, 0);
			lenv_put(f->env, nsym, builtin_list(e, a));
			lval_del(sym);
			lval_del(nsym);
			break;

		}

		/* Pop the next argument from the list */

		lval* val = lval_pop(a, 0);

		/* Bind a copy into the function's environment */

		lenv_put(f->env, sym, val);

		/* Delete symbol and value */

		lval_del(sym);
		lval_del(val);
	}

	/* Argument list is now bound so can be cleaned up */

	lval_del(a);

	/* If '&' remains in formal list it should be bound to empty list */

	if (f->formals->count > 0 && strcmp(f->formals->cell[0]->sym, "&") == 0) {

		/* Check to ensure that & is not passed invalidly. */

		if (f->formals->count != 2) {
			return lval_err("Function format invalid. Symbol '&' not followed by single symbol.");
		}

		/* Pop and delete '&' symbol */

		lval_del(lval_pop(f->formals, 0));

		/* Pop next symbol and create empty list */

		lval* sym = lval_pop(f->formals, 0);
		lval* val = lval_qexpr();

		/* Bind to environment and delete */

		lenv_put(f->env, sym, val);
		lval_del(sym);
		lval_del(val);
	}

	/* If all formals have been bound evaluate */

	if (f->formals->count == 0) {

		/* Set Function Environment parent to current evaluation Environment */

		f->env->par = e;

		/* Evaluate and return */

		return builtin_eval(f->env, lval_add(lval_sexpr(), lval_copy(f->body)));

	} else {

		/* Otherwise return partially evaluated function */

		return lval_copy(f);

	}

}

/* Evalate an s-expresssion */

lval* lval_eval_sexpr(lenv* e, lval* v) {

	/* Evaluate children */

	for (int i = 0; i < v->count; i++) {
		v->cell[i] = lval_eval(e, v->cell[i]);
	}

	/* Error checking */

	for (int i = 0; i < v->count; i++) {
		if (v->cell[i]->type == LVAL_ERR) {
			return lval_take(v, i);
		}
	}

	/* Empty expression */

	if (v->count == 0) {
		return v;
	}

	/* Single expresssion */

	if ((v->count == 1) && (v->cell[0]->type != LVAL_FUN)) {
		return lval_take(v, 0);
	}

	/* Ensure the first element is a fucntion after evaluation */

	lval* f = lval_pop(v, 0);
	if (f->type != LVAL_FUN) {

		lval* err = lval_err(
			"S-expression starts with incorrect type. Got %s, Expected %s",
			ltype_name(f->type), ltype_name(LVAL_FUN));

		lval_del(f);
		lval_del(v);

		return err;
	}

	/* Call the function to get the result */

	lval* result = lval_call(e, f, v);

	lval_del(f);

	return result;

}

/* Evaluate expressions according to their type */

lval* lval_eval(lenv* e, lval* v) {

	if (v->type == LVAL_SYM) {
		lval* x = lenv_get(e, v);
		lval_del(v);
		return x;
	}
	
	if (v->type == LVAL_SEXPR) {
		return lval_eval_sexpr(e, v);
	}

	return v;
}

/* Extract a single element from an s-expression */

lval* lval_pop(lval* v, int i) {
	if (v != NULL) {
		lval* x = v->cell[i];
		memmove(&v->cell[i], &v->cell[i+1], sizeof(lval*) * (v->count-i-1));
		v->count--;
		v->cell = realloc(v->cell, sizeof(lval*) * v->count);
		return x;
	} else {
		return lval_err("required parameters missing");
	}
}

/* Extract and delete a single element from an s-expression */

lval* lval_take(lval* v, int i) {
	lval* x = lval_pop(v, i);
	lval_del(v);
	return x;
}

/* Handle built-in operations */

lval* builtin_op(lenv* e, lval* a, char* op) {

	/* Make sure we received some arguments */

	if (a->count == 0) {
		return lval_err("Math operation called with no arguments");
	}

	/* Ensure all arguments are numbers */

	for (int i = 0; i < a->count; i++) {
		if (a->cell[i]->type != LVAL_NUM) {
			lval_del(a);
			return lval_err("Cannot operate on a non-number!");
		}
	}

	/* Pop the first element */

	lval* x = lval_pop(a, 0);

	/* If no argument and subtraction operation requested, perform unary negation */

	if ((strcmp(op, "-") == 0) && (a->count == 0)) {
		x->num = -x->num;
	}

	/* Process all remaining elements */

	while (a->count > 0) {

		/* Pop the next element */

		lval* y = lval_pop(a, 0);

		/* Perform the appropriate operation */

		if (strcmp(op, "+") == 0) {
			x->num += y->num;
		}

		if (strcmp(op, "-") == 0) {
			x->num -= y->num;
		}

		if (strcmp(op, "*") == 0) {
			x->num *= y->num;
		}

		if (strcmp(op, "/") == 0) {
			if (y->num == 0) {
				lval_del(x);
				lval_del(y);
				x = lval_err("Division by zero!");
				break;
			}
			x->num /= y->num;
		}

		if (strcmp(op, "%") == 0) {
			x->num %= y->num;
		}

		/* Delete the element now that we are done with it */

		lval_del(y);

	}

	/* Delete the input expression and return the result */

	lval_del(a);
	return x;

}

/* Handle built-in 'head' function */

lval* builtin_head(lenv* e, lval* a) {

  	LASSERT_NUM("head", a, 1);
	LASSERT_TYPE("head", a, 0, LVAL_QEXPR);
	LASSERT_NOT_EMPTY("head", a, 0)

  	lval* v = lval_take(a, 0);  
  	while (v->count > 1) {
  		lval_del(lval_pop(v, 1));
  	}

  	return v;
}

/* Handle built-in 'tail' function */

lval* builtin_tail(lenv* e, lval* a) {

  	LASSERT_NUM("tail", a, 1);
	LASSERT_TYPE("tail", a, 0, LVAL_QEXPR);
	LASSERT_NOT_EMPTY("tail", a, 0)

  	lval* v = lval_take(a, 0);  
  	lval_del(lval_pop(v, 0));

  	return v;
}

/* Handle built-in 'list' function */

lval* builtin_list(lenv* e, lval* a) {
	a->type = LVAL_QEXPR;
	return a;
}

/* Handle built-in 'eval' function */

lval* builtin_eval(lenv* e, lval* a) {

  	LASSERT_NUM("eval", a, 1);
	LASSERT_TYPE("eval", a, 0, LVAL_QEXPR);

  	lval* x = lval_take(a, 0);
  	x->type = LVAL_SEXPR;

  	return lval_eval(e, x);
}

/* Handle built-in 'join' function */

lval* builtin_join(lenv* e, lval* a) {

	for (int i = 0; i < a->count; i++) {
		LASSERT_TYPE("join", a, i, LVAL_QEXPR);
  	}

  	lval* x = lval_pop(a, 0);

  	while (a->count) {
    	x = lval_join(x, lval_pop(a, 0));
  	}

  	lval_del(a);

  	return x;
}

lval* lval_join(lval* x, lval* y) {

  	/* For each cell in 'y' add it to 'x' */

  	while (y->count) {
    	x = lval_add(x, lval_pop(y, 0));
  	}

  	/* Delete the empty 'y' and return 'x' */

  	lval_del(y);  

  	return x;
}

/* Handle built-in 'cons' function */

lval* builtin_cons(lenv* e, lval* a) {

  	LASSERT_NUM("cons", a, 2);
	LASSERT_TYPE("cons", a, 0, LVAL_QEXPR);
	LASSERT_TYPE("cons", a, 1, LVAL_QEXPR);

  	lval* x = lval_qexpr();
  	lval_add(x, lval_pop(a, 0));
  	while (a->cell[0]->count) {
  		lval_add(x, lval_pop(a->cell[0],0));
  	}
  	lval_del(a);

  	return x;
}

/* Handle built-in 'len' function */

lval* builtin_len(lenv* e, lval* a) {

  	LASSERT_NUM("len", a, 1);
	LASSERT_TYPE("len", a, 0, LVAL_QEXPR);

  	lval* x = lval_sexpr();
  	lval_add(x, lval_num(a->cell[0]->count));
  	lval_del(a);

  	return lval_eval(e, x);
}

/* Handle built-in 'init' function */

lval* builtin_init(lenv* e, lval* a) {

  	LASSERT_NUM("init", a, 1);
	LASSERT_TYPE("init", a, 0, LVAL_QEXPR);
	LASSERT_NOT_EMPTY("init", a, 0)

  	lval* v = lval_qexpr();
	while (a->cell[0]->count > 1) {
  		lval_add(v, lval_pop(a->cell[0], 0));
  	}
  	lval_del(a);

  	return v;

}

/* Provide a built-in function for defining variables */

lval* builtin_var(lenv* e, lval* a, char* func) {

	LASSERT_TYPE(func, a, 0, LVAL_QEXPR);

	/* First argument is symbol list */

	lval* syms = a->cell[0];

	/* Ensure all elements of first list are symbols */

	for (int i = 0; i < syms->count; i++) {
		LASSERT(a, (syms->cell[i]->type == LVAL_SYM),
			"Function 'def' cannot define non-symbol");
	}

	/* Check correct number of symbols and values */

	LASSERT(a, (syms->count == a->count-1),
		"Function '%s' passed too many arguments for symbols. Got %i, Expected %i.",
		func, syms->count, a->count-1);
 
	/* Assign copies of values to symbols */

	for (int i = 0; i < syms->count; i++) {
		if (strcmp(func, "def") == 0) {
			lenv_def(e, syms->cell[i], a->cell[i+1]);
		}
		if (strcmp(func, "=") == 0) {
			lenv_put(e, syms->cell[i], a->cell[i+1]);
		}
	}

	lval_del(a);

	return lval_sexpr();
}

/* Create global variables */

lval* builtin_def(lenv* e, lval* a) {
	return builtin_var(e, a, "def");
}

/* Create local variables */

lval* builtin_put(lenv* e, lval* a) {
	return builtin_var(e, a, "=");
}

/* Create a q-expression that contains a list of all defined functions and variables */

lval* builtin_vars(lenv* e, lval* a) {

  	lval* v = lval_qexpr();
	for (int i = 0; i < e->count; i++) {
		lval_add(v, lval_sym(e->syms[i]));
	}

	return v;
}

/* Handle the quit command */

lval* builtin_quit(lenv* e, lval* a) {

	// Do nothing expect pass a null built-in function

	lval* v = lval_fun(NULL);
	v->sym = "quit";

	return v;
}

/* Handle the lamda command */

lval* builtin_lamda(lenv* e, lval* a) {

	/* Validate inputs: two arguments both of which are Q-Expressions */

	LASSERT_NUM("\\", a, 2);
	LASSERT_TYPE("\\", a, 0, LVAL_QEXPR);
	LASSERT_TYPE("\\", a, 1, LVAL_QEXPR);

	/* Check that the first Q-Expression contains only symbols */

	for(int i = 0; i < a->cell[0]->count; i++) {
		LASSERT(a, (a->cell[0]->cell[i]->type == LVAL_SYM),
			"Cannot define a non-symbol. Got %s, Expected %s.",
			ltype_name(a->cell[0]->cell[i]->type), ltype_name(LVAL_SYM));
	}

	/* Pop first two arguments and pass them to lval_lambda */

	lval* formals = lval_pop(a, 0);
	lval* body = lval_pop(a, 0);
	lval_del(a);

	return lval_lambda(formals, body);
}

/* Handle testing of number order */

lval* builtin_ord(lenv* e, lval* a, char* op) {

	/* Validate inputs */

	LASSERT_NUM(op, a, 2);
	LASSERT_TYPE(op, a, 0, LVAL_NUM);
	LASSERT_TYPE(op, a, 1, LVAL_NUM);

	int r;

	if (strcmp(op, ">")  == 0) { r = (a->cell[0]->num >  a->cell[1]->num); }
	if (strcmp(op, "<")  == 0) { r = (a->cell[0]->num <  a->cell[1]->num); }
	if (strcmp(op, ">=") == 0) { r = (a->cell[0]->num >= a->cell[1]->num); }
	if (strcmp(op, "<=") == 0) { r = (a->cell[0]->num <= a->cell[1]->num); }

	lval_del(a);

	return lval_num(r);

}

/* Handle equality testing */

int lval_eq(lval* x, lval* y) {

	/* Different Types are always unequal */

	if (x->type != y->type) {
		return 0;
	}

	/* Compare Based upon type */

	switch (x->type) {

		/* Compare Number Value */

		case LVAL_NUM:
		case LVAL_BOOL:
			return (x->num == y->num);

		/* Compare String Values */

		case LVAL_ERR:
			return (strcmp(x->err, y->err) == 0);
		case LVAL_SYM:
			return (strcmp(x->sym, y->sym) == 0);

		/* If Builtin compare functions, otherwise compare formals and body */

		case LVAL_FUN:
			if (x->builtin || y->builtin) {
				return x->builtin == y->builtin;
			} else {
				return lval_eq(x->formals, y->formals) && lval_eq(x->body, y->body);
			}

		/* If list compare every individual element */

		case LVAL_QEXPR:
		case LVAL_SEXPR:
			if (x->count != y->count) {
				return 0;
			}
			for (int i = 0; i < x->count; i++) {
				/* If any element not equal then whole list not equal */
				if (!lval_eq(x->cell[i], y->cell[i])) {
					return 0;
				}
			}

			/* Otherwise lists must be equal */

			return 1;
			break;
	}

	return 0;
}

/* Implement a built-in "not" operator */

lval* builtin_not(lenv* e, lval* a) {

	LASSERT_NUM("!", a, 1);
	LASSERT_TYPE("!", a, 0, LVAL_NUM);

	lval* x = lval_pop(a, 0);

	x->num = (x->num) ? 0 : 1;

	lval_del(a);

	return x;

}

/* Handle testing logic functions */

lval* builtin_logic(lenv* e, lval* a, char* op) {

	/* Validate inputs */

	LASSERT_NUM(op, a, 2);
	LASSERT_TYPE(op, a, 0, LVAL_NUM);
	LASSERT_TYPE(op, a, 1, LVAL_NUM);

	int r;

	if (strcmp(op, "and")  == 0) { r = (a->cell[0]->num && a->cell[1]->num); }
	if (strcmp(op, "or" )  == 0) { r = (a->cell[0]->num || a->cell[1]->num); }

	lval_del(a);

	return lval_num(r);

}



/* Implement equality comparisions */

lval* builtin_cmp(lenv* e, lval* a, char* op) {

	LASSERT_NUM(op, a, 2);

	int r;

	if (strcmp(op, "==") == 0) { r =  lval_eq(a->cell[0], a->cell[1]); }
	if (strcmp(op, "!=") == 0) { r = !lval_eq(a->cell[0], a->cell[1]); }
	lval_del(a);

	return lval_num(r);
}

/* Implement if-then-else function */

lval* builtin_if(lenv* e, lval* a) {

	LASSERT_NUM("if", a, 3);
	LASSERT_TYPE("if", a, 0, LVAL_NUM);
	LASSERT_TYPE("if", a, 1, LVAL_QEXPR);
	LASSERT_TYPE("if", a, 2, LVAL_QEXPR);

	/* Mark Both Expressions as evaluable */

	lval* x;
	a->cell[1]->type = LVAL_SEXPR;
	a->cell[2]->type = LVAL_SEXPR;

	if (a->cell[0]->num) {
		/* If condition is true evaluate first expression */
		x = lval_eval(e, lval_pop(a, 1));
	} else {
		/* Otherwise evaluate second expression */
		x = lval_eval(e, lval_pop(a, 2));
	}

	/* Delete argument list and return */

	lval_del(a);

	return x;
}

/* Create built-in functions for each of the operators */

lval* builtin_add(lenv* e, lval* a) { return builtin_op(e, a, "+"); }
lval* builtin_sub(lenv* e, lval* a) { return builtin_op(e, a, "-"); }
lval* builtin_mul(lenv* e, lval* a) { return builtin_op(e, a, "*"); }
lval* builtin_div(lenv* e, lval* a) { return builtin_op(e, a, "/"); }
lval* builtin_mod(lenv* e, lval* a) { return builtin_op(e, a, "%"); }

/* Create built-in functions for each of the order tests and equality */

lval* builtin_gt(lenv* e, lval* a) { return builtin_ord(e, a, ">");  }
lval* builtin_lt(lenv* e, lval* a) { return builtin_ord(e, a, "<");  }
lval* builtin_ge(lenv* e, lval* a) { return builtin_ord(e, a, ">="); }
lval* builtin_le(lenv* e, lval* a) { return builtin_ord(e, a, "<="); }
lval* builtin_eq(lenv* e, lval* a) { return builtin_cmp(e, a, "=="); }
lval* builtin_ne(lenv* e, lval* a) { return builtin_cmp(e, a, "!="); }

/* Create built-in boolean logic functions */

lval* builtin_and(lenv* e, lval* a) { return builtin_logic(e, a, "and");  }
lval* builtin_or(lenv* e, lval* a)  { return builtin_logic(e, a, "or");   }

/* Register a new built-in function with the environment */

void lenv_add_builtin(lenv* e, char* name, lbuiltin func) {
	lval* k = lval_sym(name);
	lval* v = lval_fun(func);
	lenv_put(e, k, v);
	lval_del(k);
	lval_del(v);
}

/* Register all the currently supported built-in fuctions with the environment */

void lenv_add_builtins(lenv* e) {  

	/* List Functions */
	
	lenv_add_builtin(e, "list", builtin_list);
	lenv_add_builtin(e, "head", builtin_head);
	lenv_add_builtin(e, "tail", builtin_tail);
	lenv_add_builtin(e, "eval", builtin_eval);
	lenv_add_builtin(e, "join", builtin_join);
	lenv_add_builtin(e, "cons", builtin_cons);
	lenv_add_builtin(e, "len",  builtin_len);
	lenv_add_builtin(e, "init", builtin_init);
	lenv_add_builtin(e, "def",  builtin_def);
	lenv_add_builtin(e, "=",    builtin_put);
	lenv_add_builtin(e, "vars", builtin_vars);
	lenv_add_builtin(e, "quit", builtin_quit);
	lenv_add_builtin(e, "\\",   builtin_lamda);

	/* Mathematical Functions */

	lenv_add_builtin(e, "+",    builtin_add);
	lenv_add_builtin(e, "-",    builtin_sub);
	lenv_add_builtin(e, "*",    builtin_mul);
	lenv_add_builtin(e, "/",    builtin_div);
	lenv_add_builtin(e, "%",    builtin_mod);
	lenv_add_builtin(e, "add",  builtin_add);
	lenv_add_builtin(e, "sub",  builtin_sub);
	lenv_add_builtin(e, "mul",  builtin_mul);
	lenv_add_builtin(e, "div",  builtin_div);
	lenv_add_builtin(e, "mod",  builtin_mod);

	/* Comparision Functions */

	lenv_add_builtin(e, "if",   builtin_if);
	lenv_add_builtin(e, "==",   builtin_eq);
	lenv_add_builtin(e, "!=",   builtin_ne);
	lenv_add_builtin(e, ">",    builtin_gt);
	lenv_add_builtin(e, "<",    builtin_lt);
	lenv_add_builtin(e, ">=",   builtin_ge);
	lenv_add_builtin(e, "<=",   builtin_le);

	/* Boolean and Logic Functions */

	lenv_add_builtin(e, "!",    builtin_not);
	lenv_add_builtin(e, "not",  builtin_not);
	lenv_add_builtin(e, "and",  builtin_and);
	lenv_add_builtin(e, "or",   builtin_or);

}

/* Start REPL */

int main (int argc, char** argv) {

	/* Initialize the mpc parser for Polish Notation */

//	mpc_parser_t* Number = mpc_new("number");
//	mpc_parser_t* Bool   = mpc_new("boolean");
//	mpc_parser_t* Symbol = mpc_new("symbol");
//	mpc_parser_t* Sexpr  = mpc_new("sexpr");
//	mpc_parser_t* Qexpr  = mpc_new("qexpr");
//	mpc_parser_t* Expr   = mpc_new("expr");
//	mpc_parser_t* Lispy  = mpc_new("lispy");

//	mpca_lang(MPCA_LANG_DEFAULT,
//  		" \
//    		number : /-?[0-9]+/ ; \
//    		boolean: /^(true|false)$/ ; \
//    		symbol : /[a-zA-Z0-9_+\\-*\\/\\\\=<>!&%]+/ ; \
//    		sexpr  : '(' <expr>* ')' ; \
//    		qexpr  : '{' <expr>* '}' ; \
//    		expr   : <number> | <boolean> | <symbol> | <sexpr> | <qexpr> ; \
//    		lispy  : /^/ <expr>* /$/ ; \
//  		",
//  		Number, Bool, Symbol, Sexpr, Qexpr, Expr, Lispy);

	/* Register built-in functions */

	lenv* e = lenv_new();
	lenv_add_builtins(e);

	/* Display Initialization Header */

        puts("\x1B[2J\x1B[f");
        puts("ptscheme v0.0.1");
	puts("Enter 'quit' to exit\n");

	/* Enter into REPL */

	bool repeatREPL = true;

	while (repeatREPL) {

		char* input = readline("> ");
		add_history(input);

                puts(input);

//		mpc_result_t r;

//		if (mpc_parse("<stdin>", input, Lispy, &r)) {
//			lval* x = lval_eval(e, lval_read(r.output));
//  			repeatREPL = ((x->type != LVAL_FUN) || (strcmp(x->sym, "quit") != 0));
//			if (repeatREPL) {
//				lval_println(x);
//				lval_del(x);
//			}
//			mpc_ast_delete(r.output);
//		} else {
//			mpc_err_print(r.error);
//			mpc_err_delete(r.error);
//		}
		free(input);
	}

	/* Clean up and go home now that the hard work is done */

	lenv_del(e);

//	mpc_cleanup(7, Number, Bool, Symbol, Sexpr, Qexpr, Expr, Lispy);

	return EXIT_SUCCESS;
}

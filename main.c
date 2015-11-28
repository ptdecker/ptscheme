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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <editline/readline.h>

/* Global vars */

char *token;

/* Lex List */

void lex_list() {
	printf("%s\n", "left parenthesis");
}

/* Lexer */

void lex() {
 	if (c[0] = '(') {
 		lex_list();
 	} else {
	 	printf("%s\n", token);
	 	token = strtok(NULL, " ");
	 	if (token != NULL) {
 			lex();
 		}
 	}
}

 /* Main */

int main (int argc, char** argv) {

	char* input;

	/* Set up readline */

	rl_bind_key('\t', rl_complete); // Config to auto-complete paths when tab key is hit

	/* Display Initialization Header */

    fputs("\x1B[2J\x1B[f", stdout);
    fputs("ptscheme v0.0.1\n", stdout);
	fputs("Enter 'quit' to exit\n\n", stdout);

	/* Enter into REPL */

	while (true) {

		// Display prompt and read input (note: input must be freed after use)

		input = readline("> ");

		// Check for EOF

		if (!input) break;

		// Add input to history

		add_history(input);

		// Let's get started!

		token = strtok(input, " ");
		lex();

		// Free input

		free(input);
	}

	/* Clean up and go home now that the hard work is done */

	return EXIT_SUCCESS;
}

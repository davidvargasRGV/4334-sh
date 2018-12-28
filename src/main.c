/*
 * src/main.c -- shell entry point
 *
 * David Vargas
 * CSCI 4334-01 Operating Systems
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 * Programming Project One
 * Due 09/30/16
 *
 */

/* standard c header files */
#include <stdio.h>
#include <stdlib.h>

/* shell specific header files */
#include <sh/main.h>
#include <sh/console.h>
#include <sh/parser.h>
#include <sh/exec.h>

void sh_free(struct shell_t **shell)
{
	/* shell */
	if ((*shell) != NULL) {
		/* shell->exec */
		if ((*shell)->exec != NULL) {
			exec_free(&((*shell)->exec));
		}

		/* shell->parser */
		if ((*shell)->parser != NULL) {
			parser_free(&((*shell)->parser));
		}

		/* shell->console */
		if ((*shell)->console != NULL) {
			console_free(&((*shell)->console));
		}

		free((*shell));
	}
}

void sh_main(struct shell_t **shell)
{
	/* loop until exit command */
	while ((*shell)->status == 1) {
		console_read(&(*shell));
	}
}

void sh_alloc(struct shell_t **shell)
{
	(*shell) = (struct shell_t *) malloc(sizeof(struct shell_t));

	if ((*shell) == NULL) {
		fprintf(stderr, "sh: error: malloc() failed\n");
		abort();
	}

	(*shell)->status = 1;
	(*shell)->console = NULL;
	(*shell)->parser = NULL;
	(*shell)->exec = NULL;

	console_alloc(&((*shell)->console));
	parser_alloc(&((*shell)->parser));
	exec_alloc(&((*shell)->exec));
}

int main()
{
	struct shell_t *shell;

	shell = NULL;

	sh_alloc(&(shell));

	sh_main(&(shell));

	sh_free(&(shell));

	return 0;
}

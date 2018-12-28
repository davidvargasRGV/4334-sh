/*
 * src/console.c -- shell interface to GNU readline
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
#include <string.h>

/* GNU readline specific header files */
#include <readline/readline.h>
#include <readline/history.h>

/* shell specific header files */
#include <sh/main.h>
#include <sh/console.h>
#include <sh/parser.h>
#include <sh/exec.h>

void console_free(struct console_t **console)
{
	/* free console */
	if ((*console) != NULL) {
		/* free console->ps2 */
		if ((*console)->ps2 != NULL) {
			free((*console)->ps2);
		}

		/* free console->ps1 */
		if ((*console)->ps1 != NULL) {
			free((*console)->ps1);
		}

		/* free console->line */
		if ((*console)->line != NULL) {
			free((*console)->line);
		}

		free((*console));
	}
}

void console_setup(struct shell_t **shell)
{
	if ((*shell)->console->line != NULL) {
		free((*shell)->console->line);
	}
}

void console_read(struct shell_t **shell)
{
	/* setup console */
	console_setup(&(*shell));

	/* read line from GNU readline */
	(*shell)->console->line = readline((*shell)->console->ps1);

	/* EOF encountered while reading a line and the line is empty */
	if ((*shell)->console->line == NULL) {
		return;
	}

	/* lines which begin with a space are not saved in the history list */
	if (strlen((*shell)->console->line) > 0 && (*shell)->console->line[0] != ' ') {
		add_history((*shell)->console->line);
	}

	/* parse the line */
	parse(&(*shell));

	/* execute the line */
	exec(&(*shell));
}

void console_alloc(struct console_t **console)
{
	size_t size;

	(*console) = (struct console_t *) malloc(sizeof(struct console_t));

	if ((*console) == NULL) {
		fprintf(stderr, "sh: error: malloc() failed\n");
		abort();
	}

	/* console->line setup */
	(*console)->line = NULL;

	/* console->ps1 setup */
	size = (strlen(DEFAULT_PS1) * sizeof(char)) + sizeof(char);

	(*console)->ps1 = (char *) malloc(size);

	if ((*console)->ps1 == NULL) {
		fprintf(stderr, "sh: error: malloc() failed\n");
		abort();
	}

	memset((*console)->ps1, 0, size);

	size = size - sizeof(char);
	strncpy((*console)->ps1, DEFAULT_PS1, size);

	/* console->ps2 setup */
	size = (strlen(DEFAULT_PS2) * sizeof(char)) + sizeof(char);

	(*console)->ps2 = (char *) malloc(size);

	if ((*console)->ps2 == NULL) {
		fprintf(stderr, "sh: error: malloc() failed\n");
		abort();
	}

	memset((*console)->ps2, 0, size);

	size = size - sizeof(char);
	strncpy((*console)->ps2, DEFAULT_PS2, size);
}

/*
 * src/parser.c -- shell parser
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

void parser_free(struct parser_t **parser)
{
	struct token_t *tp;

	int i;

	/* free parser */
	if ((*parser) != NULL) {
		/* free parser->path */
		if ((*parser)->path != NULL) {
			/* free parser->path->argv[i] */
			for (i = 0; i < (*parser)->path->argc; i++) {
				if ((*parser)->path->argv[i] != NULL) {
					free((*parser)->path->argv[i]);
				}
			}

			/* free parser->path->argv */
			if ((*parser)->path->argv != NULL) {
				free((*parser)->path->argv);
			}

			free((*parser)->path);
		}

		/* free parser->paths */
		if ((*parser)->paths != NULL) {
			/* free parser->paths->next */
			while ((*parser)->paths->token != NULL) {
				tp = (*parser)->paths;

				(*parser)->paths = (*parser)->paths->next;

				if (tp != NULL) {
					free(tp);
				}
			}

			free((*parser)->paths);
		}

		/* free parser->command */
		if ((*parser)->command != NULL) {
			/* free parser->command->argv[i] */
			for (i = 0; i < (*parser)->command->argc; i++) {
				if ((*parser)->command->argv[i] != NULL) {
					free((*parser)->command->argv[i]);
				}
			}

			/* free parser->command->argv */
			if ((*parser)->command->argv != NULL) {
				free((*parser)->command->argv);
			}

			free((*parser)->command);
		}

		/* free parser->tokens */
		if ((*parser)->tokens != NULL) {
			/* free parser->tokens->next */
			while ((*parser)->tokens->token != NULL) {
				tp = (*parser)->tokens;

				(*parser)->tokens = (*parser)->tokens->next;

				if (tp != NULL) {
					free(tp);
				}
			}

			free((*parser)->tokens);
		}

		free((*parser));
	}
}

void parse_path_argv(struct shell_t **shell)
{
	struct token_t *tp;

	size_t size;

	int i;

	tp = (*shell)->parser->paths;

	/* path->argv */
	size = ((*shell)->parser->path->argc * sizeof(char *)) + sizeof(char *);

	(*shell)->parser->path->argv = (char **) malloc(size);

	if ((*shell)->parser->path->argv == NULL) {
		fprintf(stderr, "sh: error: malloc() failed\n");
		abort();
	}

	memset((*shell)->parser->path->argv, 0, size);

	/* path->argv[i] */
	for (i = 0; i < (*shell)->parser->path->argc; i++) {
		size = (strlen(tp->token) * sizeof(char)) + sizeof(char);

		(*shell)->parser->path->argv[i] = (char *) malloc(size);

		if ((*shell)->parser->path->argv[i] == NULL) {
			fprintf(stderr, "sh: error: malloc() failed\n");
			abort();
		}

		memset((*shell)->parser->path->argv[i], 0, size);

		size = size - sizeof(char);
		strncpy((*shell)->parser->path->argv[i], tp->token, size);

		tp = tp->next;
	}
}

void parse_path_list(struct shell_t **shell)
{
	size_t size;

	struct token_t *tp;

	/* exec->orig_path */
	(*shell)->exec->orig_path = getenv("PATH");

	/* exec->shell_path */
	size = (strlen((*shell)->exec->orig_path) * sizeof(char)) + sizeof(char);

	(*shell)->exec->shell_path = (char *) malloc(size);

	if ((*shell)->exec->shell_path == NULL) {
		fprintf(stderr, "sh: error: malloc() failed\n");
		abort();
	}

	memset((*shell)->exec->shell_path, 0, size);

	size = size - sizeof(char);
	strncpy((*shell)->exec->shell_path, (*shell)->exec->orig_path, size);

	/* parser->paths */
	tp = (*shell)->parser->paths;

	tp->token = strtok((*shell)->exec->shell_path, PATH_DELIM);

	while (tp->token != NULL) {
		(*shell)->parser->path->argc++;

		tp->next = (struct token_t *) malloc(sizeof(struct token_t));

		if (tp->next == NULL) {
			fprintf(stderr, "sh: error: malloc() failed\n");
			abort();
		}

		tp = tp->next;

		tp->token = strtok(NULL, PATH_DELIM);
	}
}

void parse_command_argv(struct shell_t **shell)
{
	struct token_t *tp;

	size_t size;

	int i;

	tp = (*shell)->parser->tokens;

	/* command->argc == 0 -- empty line */
	if ((*shell)->parser->command->argc == 0) {
		return;
	}

	/* command->argv */
	size = ((*shell)->parser->command->argc * sizeof(char *)) + sizeof(char *);

	(*shell)->parser->command->argv = (char **) malloc(size);

	if ((*shell)->parser->command->argv == NULL) {
		fprintf(stderr, "sh: error: malloc() failed\n");
		abort();
	}

	memset((*shell)->parser->command->argv, 0, size);

	/* command->argv[i] */
	for (i = 0; i < (*shell)->parser->command->argc; i++) {
		size = (strlen(tp->token) * sizeof(char)) + sizeof(char);

		(*shell)->parser->command->argv[i] = (char *) malloc(size);

		if ((*shell)->parser->command->argv[i] == NULL) {
			fprintf(stderr, "sh: error: malloc() failed\n");
			abort();
		}

		memset((*shell)->parser->command->argv[i], 0, size);

		size = size - sizeof(char);
		strncpy((*shell)->parser->command->argv[i], tp->token, size);

		tp = tp->next;
	}
}

void parse_command_list(struct shell_t **shell)
{
	struct token_t *tp;

	tp = (*shell)->parser->tokens;

	/* parser->tokens */
	tp->token = strtok((*shell)->console->line, WHITESPACE);

	while (tp->token != NULL) {
		(*shell)->parser->command->argc++;

		tp->next = (struct token_t *) malloc(sizeof(struct token_t));

		if (tp->next == NULL) {
			fprintf(stderr, "sh: error: malloc() failed\n");
			abort();
		}

		tp = tp->next;

		tp->token = strtok(NULL, WHITESPACE);
	}
}

void parse_setup(struct shell_t **shell)
{
	struct token_t *tp;

	int i;

	/* parser->tokens */
	while ((*shell)->parser->tokens->token != NULL) {
		tp = (*shell)->parser->tokens;

		(*shell)->parser->tokens = (*shell)->parser->tokens->next;

		if (tp != NULL) {
			free(tp);
		}
	}

	(*shell)->parser->tokens->token = NULL;
	(*shell)->parser->tokens->next = NULL;

	/* parser->command */
	for (i = 0; i < (*shell)->parser->command->argc; i++) {
		if ((*shell)->parser->command->argv[i] != NULL) {
			free((*shell)->parser->command->argv[i]);
		}
	}

	if ((*shell)->parser->command->argv != NULL) {
		free((*shell)->parser->command->argv);
	}

	(*shell)->parser->command->argc = 0;
	(*shell)->parser->command->argv = NULL;

	/* parser->paths */
	while ((*shell)->parser->paths->token != NULL) {
		tp = (*shell)->parser->paths;

		(*shell)->parser->paths = (*shell)->parser->paths->next;

		if (tp != NULL) {
			free(tp);
		}
	}

	(*shell)->parser->paths->token = NULL;
	(*shell)->parser->paths->next = NULL;

	/* parser->path */
	for (i = 0; i < (*shell)->parser->path->argc; i++) {
		if ((*shell)->parser->path->argv[i] != NULL) {
			free((*shell)->parser->path->argv[i]);
		}
	}

	if ((*shell)->parser->path->argv != NULL) {
		free((*shell)->parser->path->argv);
	}

	(*shell)->parser->path->argc = 0;
	(*shell)->parser->path->argv = NULL;

	/* exec->shell_path */
	if ((*shell)->exec->shell_path != NULL) {
		free((*shell)->exec->shell_path);
	}

	(*shell)->exec->shell_path = NULL;
}

void parse(struct shell_t **shell)
{
	/* setup parser */
	parse_setup(&(*shell));

	/* create command list from line */
	parse_command_list(&(*shell));

	/* create command argv from command list */
	parse_command_argv(&(*shell));

	/* create path list from $PATH */
	parse_path_list(&(*shell));

	/* create path argv from path list */
	parse_path_argv(&(*shell));
}

void parser_alloc(struct parser_t **parser)
{
	(*parser) = (struct parser_t *) malloc(sizeof(struct parser_t));

	if ((*parser) == NULL) {
		fprintf(stderr, "sh: error: malloc() failed\n");
		abort();
	}

	/* parser->tokens setup */
	(*parser)->tokens = (struct token_t *) malloc(sizeof(struct token_t));

	if ((*parser)->tokens == NULL) {
		fprintf(stderr, "sh: error: malloc() failed\n");
		abort();
	}

	(*parser)->tokens->token = NULL;
	(*parser)->tokens->next = NULL;

	/* parser->command setup */
	(*parser)->command = (struct command_t *) malloc(sizeof(struct command_t));

	if ((*parser)->command == NULL) {
		fprintf(stderr, "sh: error: malloc() failed\n");
		abort();
	}

	(*parser)->command->argc = 0;
	(*parser)->command->argv = NULL;

	/* parser->paths setup */
	(*parser)->paths = (struct token_t *) malloc(sizeof(struct token_t));

	if ((*parser)->paths == NULL) {
		fprintf(stderr, "sh: error: malloc() failed\n");
		abort();
	}

	(*parser)->paths->token = NULL;
	(*parser)->paths->next = NULL;

	/* parser->path setup */
	(*parser)->path = (struct command_t *) malloc(sizeof(struct command_t));

	if ((*parser)->path == NULL) {
		fprintf(stderr, "sh: error: malloc() failed\n");
		abort();
	}

	(*parser)->path->argc = 0;
	(*parser)->path->argv = NULL;
}

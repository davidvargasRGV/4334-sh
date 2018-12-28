/*
 * src/exec.c -- shell command execution
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
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* shell specific header files */
#include <sh/main.h>
#include <sh/console.h>
#include <sh/parser.h>
#include <sh/exec.h>

void exec_free(struct exec_t **exec)
{
	/* free exec */
	if ((*exec) != NULL) {
		/* free exec->path */
		if ((*exec)->path != NULL) {
			free((*exec)->path);
		}

		/* free exec->shell_path */
		if ((*exec)->shell_path != NULL) {
			free((*exec)->shell_path);
		}

		free((*exec));
	}
}

void exec_search_paths(struct shell_t **shell)
{
	int i;

	size_t size;
	size_t cmdsize;
	size_t fullsize;

	char *pathname;

	cmdsize = strlen((*shell)->parser->command->argv[0]) * sizeof(char);

	for (i = 0; i < (*shell)->parser->path->argc; i++) {
		size = (strlen((*shell)->parser->path->argv[i]) * sizeof(char)) + sizeof(char);
		fullsize = size + sizeof(char) + cmdsize;

		pathname = (char *) malloc(fullsize);

		if (pathname == NULL) {
			fprintf(stderr, "sh: error: malloc() failed\n");
			abort();
		}

		memset(pathname, 0, fullsize);

		size = size - sizeof(char);
		strncpy(pathname, (*shell)->parser->path->argv[i], size);
		strncat(pathname, "/", sizeof(char));
		strncat(pathname, (*shell)->parser->command->argv[0], cmdsize);

		/* exec->mode */
		if (access(pathname, X_OK) == 0) {
			(*shell)->exec->mode = XOK;

			size = (strlen(pathname) * sizeof(char)) + sizeof(char);

			(*shell)->exec->path = (char *) malloc(size);

			if ((*shell)->exec->path == NULL) {
				fprintf(stderr, "sh: error: malloc() failed\n");
				abort();
			}

			memset((*shell)->exec->path, 0, size);

			size = size - sizeof(char);
			strncpy((*shell)->exec->path, pathname, size);

			break;
		} else if (access(pathname, F_OK) == 0) {
			(*shell)->exec->mode = FOK;

			break;
		}

		free(pathname);
	}
}

void exec_search_pathname(struct shell_t **shell)
{
	size_t size;

	/* exec->mode */
	if (access((*shell)->parser->command->argv[0], X_OK) == 0) {
		(*shell)->exec->mode = XOK;

		size = (strlen((*shell)->parser->command->argv[0]) * sizeof(char)) + sizeof(char);

		(*shell)->exec->path = (char *) malloc(size);

		if ((*shell)->exec->path == NULL) {
			fprintf(stderr, "sh: error: malloc() failed\n");
			abort();
		}

		memset((*shell)->exec->path, 0, size);

		size = size - sizeof(char);
		strncpy((*shell)->exec->path, (*shell)->parser->command->argv[0], size);
	} else if (access((*shell)->parser->command->argv[0], F_OK) == 0) {
		(*shell)->exec->mode = FOK;
	}
}

void exec_setup(struct shell_t **shell)
{
	/* exec->path */
	if ((*shell)->exec->path != NULL) {
		free((*shell)->exec->path);
	}

	(*shell)->exec->path = NULL;

	/* exec->mode */
	(*shell)->exec->mode = NOK;
}

void exec(struct shell_t **shell)
{
	/* command->argc == 0 -- empty line */
	if ((*shell)->parser->command->argc == 0) {
		return;
	}

	/* setup shell execution */
	exec_setup(&(*shell));

	/* absolute pathname */
	if ((*shell)->parser->command->argv[0][0] == '/') {
		exec_search_pathname(&(*shell));
	} else {
		/* search $PATH for command */
		exec_search_paths(&(*shell));
	}

	/* exit command */
	if (strncmp((*shell)->parser->command->argv[0], "exit", strlen((*shell)->parser->command->argv[0])) == 0) {
		(*shell)->status = 0;
		return;
	}

	/* execute command in a child process */
	if(fork() == 0) {
		/* exec->mode */
		if ((*shell)->exec->mode == NOK) {
			printf("%s: command not found\n", (*shell)->parser->command->argv[0]);
		} else if ((*shell)->exec->mode == FOK) {
			printf("sh: %s: Permission denied\n", (*shell)->parser->command->argv[0]);
		} else {
			execv((*shell)->exec->path, (*shell)->parser->command->argv);
		}

		exit(0);
	}

	/* wait for child process */
	wait(NULL);
}

void exec_alloc(struct exec_t **exec)
{
	/* exec setup */
	(*exec) = (struct exec_t *) malloc(sizeof(struct exec_t));

	if ((*exec) == NULL) {
		fprintf(stderr, "sh: error: malloc() failed\n");
		abort();
	}

	/* exec->orig_path setup */
	(*exec)->orig_path = NULL;

	/* exec->shell_path setup */
	(*exec)->shell_path = NULL;

	/* exec->full_path setup */
	(*exec)->path = NULL;

	/* exec->mode setup */
	(*exec)->mode = NOK;
}

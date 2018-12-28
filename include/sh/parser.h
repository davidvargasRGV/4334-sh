/*
 * include/sh/parser.h -- shell parser
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

#ifndef _PARSER_H
#define _PARSER_H

#define WHITESPACE	" \t\r\n\v\f"	/* space, tab, carriage return, newline, vertical tab, formfeed */
#define PATH_DELIM	":"		/* $PATH delimiter */

struct command_t {
	int argc;			/* argument count */
	char **argv;			/* arguments */
};

struct token_t {
	char *token;			/* token */
	struct token_t *next;		/* next token */
};

struct parser_t {
	struct token_t *tokens;		/* command tokens */
	struct command_t *command;	/* command */
	struct token_t *paths;		/* path tokens */
	struct command_t *path;		/* path */
};

void parser_alloc(struct parser_t **parser);
void parse(struct shell_t **shell);
void parser_free(struct parser_t **parser);

#endif

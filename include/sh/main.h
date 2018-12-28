/*
 * include/sh/main.h -- shell entry point
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

#ifndef _MAIN_H
#define _MAIN_H

struct shell_t {
	int status;			/* shell runtime status */
	struct console_t *console;	/* shell interface to GNU readline */
	struct parser_t *parser;	/* shell parser */
	struct exec_t *exec;		/* shell command execution */
};

#endif

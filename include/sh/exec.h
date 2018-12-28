/*
 * include/sh/exec.h -- shell command execution
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

#ifndef _EXEC_H
#define _EXEC_H

#define NOK	0
#define FOK	1
#define ROK	2
#define WOK	4
#define XOK	8

struct exec_t {
	char *orig_path;		/* original $PATH */
	char *shell_path;		/* duplicate $PATH */
	char *path;			/* full command path */
	int mode;			/* access mode */
};

void exec_alloc(struct exec_t **exec);
void exec(struct shell_t **shell);
void exec_free(struct exec_t **exec);

#endif

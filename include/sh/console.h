/*
 * include/sh/console.h -- shell interface to GNU readline
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

#ifndef _CONSOLE_H
#define _CONSOLE_H

#define DEFAULT_PS1	"sh-0.0.1$ "
#define DEFAULT_PS2	"> "

struct console_t {
	char *line;	/* text of the line read by GNU readline */

	char *ps1;	/* prompt string 1 */
	char *ps2;	/* prompt string 2 */
};

void console_alloc(struct console_t **console);
void console_read(struct shell_t **shell);
void console_free(struct console_t **console);

#endif

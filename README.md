# 4334-sh

This is the source code for the Simple Shell assignment for CSCI 4334 Operating Systems at the University of Texas Rio Grande Valley.

The textbook for the course was **Operating Systems - A Modern Perspective** by Gary Nutt.

## Building

A makefile is provided. Simply execute the following...

```
make
```

The following is the directory structure...

* bin - C object files and Shell binary
* include/sh - C header files
* src - C source files

**Note**: GNU readline and ncurses are required.

## Running

To execute the Shell, simply execute the following... 

```
./bin/sh
```

The following builtin commands are accepted...

* *exit*

**Note**: help and other usual builtin commands were not required, including pipes, file redirection, or other such features.

## Example

To execute the shell, simply execute the following...

```
./bin/sh
<execute any commands within $PATH>
exit
```

## Authors

* David Vargas (*david.vargas.rgv [AT] gmail.com*)

## License

All rights reserved.

## Acknowledgments

None

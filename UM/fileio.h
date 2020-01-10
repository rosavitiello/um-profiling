/*
 * fileio.h
 * COMP 40 Fall 2019 um
 * 
 * fileio interface. Opens and closes files and detects if file is stdin
 * 
 * Written by:    Rosa Vitiello, Ethan Schreiber
 * Last modified: Nov. 19, 2019
 */

/* -------------------------------- includes -------------------------------- */

#include <stdio.h>
#include <stdlib.h>

#ifndef FILEIO_INCLUDED
#define FILEIO_INCLUDED

/* ------------------------------- prototypes ------------------------------- */

FILE *open_file(const char *fname);

void close_file(FILE *fp, const char *fname);

#endif
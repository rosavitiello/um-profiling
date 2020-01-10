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

/* 
 * Function: open_file
 * Purpose:  opens file or assigns file to stdin depending on case
 * Input:    const char *fname
 * Output:   FILE *fp
 * Notes:    error if file cannot be opened 
 */ 
static inline FILE *open_file(const char *fname) 
{

    FILE *file;
    if (fname != NULL) {            /* if no file, take from stdin */
        file = fopen(fname, "rb");
        if (file == NULL) {
            perror(fname);
            exit(EXIT_FAILURE);
        }
    }
    else {
        file = stdin;
    }

    return file;
}

/* 
 * Function: close_file
 * Purpose:  closes fp if not stdin
 * Input:    FILE *fp, const char *fname
 * Output:   none
 * Notes:    error if file cannot be closed properly  
 */ 
static inline void close_file(FILE *fp, const char *fname) 
{    
    if (fp != stdin) {           /* free file if fp not stdin */
        if (fclose(fp) != 0) {
            perror(fname);
            exit(EXIT_FAILURE);
        }
    }
}

#endif
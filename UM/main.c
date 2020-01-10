/*
 * main.c
 * COMP 40 Fall 2019 um
 * 
 * Main for UM implementation. Handles File I/O & Runs UM using operation module
 * 
 * Written by:    Rosa Vitiello, Ethan Schreiber
 * Last modified: Nov. 17, 2019
 */

/* -------------------------------- includes -------------------------------- */

#include <stdio.h>
#include <stdlib.h>

#include "operation.h"
#include "fileio.h"

/* ---------------------------------- main ---------------------------------- */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        /* needs precisely 1 file arg */
        exit(EXIT_FAILURE);
    }

    FILE *file = open_file(argv[1]);
    run(file);
    close_file(file, argv[1]);
}
/*
 * operation.h
 * COMP 40 Fall 2019 um
 * 
 * Header file for operation module
 * 
 * Written by:    Rosa Vitiello, Ethan Schreiber
 * Last modified: Nov. 17, 2019
 */

/* -------------------------------- includes -------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "instruction.h"

#include <seq.h>
#include <bitpack.h>
#include <assert.h>

#ifndef OPERATION_INCLUDED
#define OPERATION_INCLUDED

/* ------------------------------- prototypes ------------------------------- */

/* ------------------------------ definitions ------------------------------- */

#define BYTESIZE 8
#define INIT_LSB 24

#define CODE_LSB 28
#define CODE_WIDTH 4

#define LOADVAL_LSB 25
#define A_LSB 6
#define B_LSB 3
#define C_LSB 0
#define REG_WIDTH 3

#define VAL_LSB 0
#define VAL_WIDTH 25



/* ------------------------- function definitions --------------------------- */

/*
 * Function: init
 * Input:    file * fp
 * Return:   Seq_T with init 0
 * Purpose:  read instructions from file and store in seg 0
 */
static inline Seq_T init(FILE *fp)
{
    assert(fp != NULL);
    
    Seq_T instr = Seq_new(1);
    int index = INIT_LSB;

    /*set up first word for reading*/
    uint32_t *word = malloc(sizeof(uint32_t));
    assert(word != NULL);
    *word = 0;

    /* read from file into array */
    int c = getc(fp);
    while(c != EOF) {
        /* pack next byte */
        *word = Bitpack_newu(*word, BYTESIZE, index, c);
        index -= BYTESIZE;
        if (index < 0) {
            /* word complete, add to sequence */
            Seq_addhi(instr, word);
            index = INIT_LSB;
            /* make new word */
            word = malloc(sizeof(uint32_t));
            assert(word != NULL);
            *word = 0;
        }
        /* get next byte */
        c = getc(fp);
    }

    /* make sequences */
    new_mem();
    Seq_T unmap = Seq_new(1);

    /* copy from array to memory */
    int instr_len = Seq_length(instr);
    map_segment(unmap, instr_len);
    for (int i = 0; i < instr_len; i++) {
        uint32_t *val = (uint32_t *)Seq_remlo(instr);
        set_val(0, i, *val);
        free(val);
    }


    free(word);
    Seq_free(&instr);
    return unmap;
}

/*
 * Function: run
 * Input:    file * fp
 * Return:   none
 * Purpose:  initializes and runs um!
 */
static inline void run(FILE *fp)
{
    assert(fp != NULL);

    int prog_ctr = 0;

    Seq_T unmap = init(fp);
    int code = 0;
    int a = 0;
    int b = 0;
    int c = 0;
    int val = 0;
    
    while (code != 7) {
        /* get instruction from memory*/
        //fprintf(stderr, "memory len %d\n", memory_len);
        uint32_t instr = get_val(0, prog_ctr);
        
        /* parse instr */
        code = Bitpack_getu(instr, CODE_WIDTH, CODE_LSB);
        if (code == 13) { /* loadval instruction */
            a = Bitpack_getu(instr, REG_WIDTH, LOADVAL_LSB);
            val = Bitpack_getu(instr, VAL_WIDTH, VAL_LSB);
        } else {
            a = Bitpack_getu(instr, REG_WIDTH, A_LSB);
            b = Bitpack_getu(instr, REG_WIDTH, B_LSB);
            c = Bitpack_getu(instr, REG_WIDTH, C_LSB);
        }

        /* increment program counter */
        prog_ctr++;
        
        /* run instr */
        switch (code){
            case 0:
                conditional_move(a, b, c);
                break;
            case 1:
                seg_load(a, b, c);
                break;
            case 2:
                seg_store( a, b, c);
                break;
            case 3:
                addition(a, b, c);
                break;
            case 4:
                multiplication(a, b, c);
                break;
            case 5:
                division(a, b, c);
                break;
            case 6:
                bitwise_nand(a, b, c);
                break;
            case 7:
                halt(unmap);
                break;
            case 8:
                //fprintf(stderr, "seq %p\n", (void *)seq);
                map_seg(unmap, b, c);
                //fprintf(stderr, "seq %p\n", (void *)seq);
                break;
            case 9:
                unmap_seg(unmap, c);
                break;
            case 10:
                output(c);
                break;
            case 11:
                input(c);
                break;
            case 12:
                prog_ctr = reg[c]; /* set prog ctr to reg c */
                load_prog(unmap, b);
                break;
            case 13:
                load_val(a, val);
                break;
        }
    }
}

#endif

/*
 * instruction.h
 * COMP 40 Fall 2019 um
 * 
 * Header file for instructions functions
 * 
 * Written by:    Rosa Vitiello, Ethan Schreiber
 * Last modified: Nov. 16, 2019
 */

/* -------------------------------- includes -------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <seq.h>
#include "memory.h"

#include <assert.h>
#include <stdbool.h>

#ifndef INSTRUCTION_INCLUDED
#define INSTRUCTION_INCLUDED

/* ------------------------------- prototypes ------------------------------- */

/* ------------------------------- globals ---------------------------------- */
uint32_t reg[8];

/* ------------------------- function definitions --------------------------- */

/*
 * Function: conditional_move
 * Input:    register data structure, three ints 0-7
 * Return:   none
 * Purpose:  perform the following: if c != 0, a = b 
 */
static inline void conditional_move(int a, int b, int c)
{

    assert(a >= 0 && a < 8);
    assert(b >= 0 && b < 8);
    assert(c >= 0 && c < 8);


    if (reg[c] != 0) {
        uint32_t val = reg[b];
        reg[a] = val;
    }
}

/*
 * Function: seg_load 
 * Input:    memory and register array, three ints 0-7
 * Return:   none
 * Purpose:  loads value from segemented memory in mem[b][c] into reg[a]
 * Notes:    CRE if unmapped segment accessed
 */
static inline void seg_load(int a, int b, int c)
{
    assert(a >= 0 && a < 8);
    assert(b >= 0 && b < 8);
    assert(c >= 0 && c < 8);
    
    uint32_t val = get_val(reg[b], reg[c]);
    reg[a] = val;
}

/*
 * Function: seg_store
 * Input:    memory and register data structures, three ints 0-7
 * Return:   none
 * Purpose:  stores value from reg[c] in mem[a][b]
 * Notes:    CRE if unmapped segment accessed
 */
static inline void seg_store(int a, int b, int c)
{
    assert(a >= 0 && a < 8);
    assert(b >= 0 && b < 8);
    assert(c >= 0 && c < 8);

    uint32_t val = reg[c];
    set_val(reg[a], reg[b], val);
}

/*
 * Function: addition
 * Input:    register array, three ints 0-7
 * Return:   none
 * Purpose:  performs addition operation, r[a] = (r[b] + r[c]) mod 2^32
 */
static inline void addition(int a, int b, int c)
{
    assert(a >= 0 && a < 8);
    assert(b >= 0 && b < 8);
    assert(c >= 0 && c < 8);

    uint32_t val1 = reg[c];
    uint32_t val2 = reg[b];
    val1 = val2 + val1; /* overflow performs mod operation */
    reg[a] = val1;
}

/*
 * Function: multiplication
 * Input:    register array, three ints 0-7
 * Return:   none
 * Purpose:  performs multiplication operation, r[a] = (r[b] x r[c]) mod 2^32
 */
static inline void multiplication(int a, int b, int c)
{
    assert(a >= 0 && a < 8);
    assert(b >= 0 && b < 8);
    assert(c >= 0 && c < 8);

    uint32_t val1 = reg[c];
    uint32_t val2 = reg[b];
    val1 = val2 * val1; /* overflow performs mod operation */
    reg[a] = val1;
}

/*
 * Function: division
 * Input:    register data structure, three ints 0-7
 * Return:   none
 * Purpose:  performs r[a] = r[b] / r[c]
 * Notes:    CRE if divide by zero
 */
static inline void division(int a, int b, int c)
{
    assert(a >= 0 && a < 8);
    assert(b >= 0 && b < 8);
    assert(c >= 0 && c < 8);
    
    uint32_t val1 = reg[c];
    uint32_t val2 = reg[b];
    assert(val1 != 0);  /* CRE if divide by zero */

    val1 = val2 / val1;
    reg[a] = val1;
}


/*
 * Function: bitwise nand
 * Input:    register array, three ints 0-7
 * Return:   none
 * Purpose:  performs bitwise "not" and operation, r[a] = ~(r[b] & r[c])
 */
static inline void bitwise_nand(int a, int b, int c)
{
    assert(a >= 0 && a < 8);
    assert(b >= 0 && b < 8);
    assert(c >= 0 && c < 8);

    uint32_t val1 = reg[c]; 
    uint32_t val2 = reg[b];
    val1 = ~(val2 & val1);
    reg[a] = val1;
}

/*
 * Function: halt
 * Input:    register and memory data structures
 * Return:   none
 * Purpose:  frees memory and halts program
 */
static inline void halt(Seq_T unmap)
{
    assert(unmap != NULL);

    free_mem(unmap);
}

/*
 * Function: map_seg
 * Input:    memory and register array, two ints 0-7
 * Return:   none
 * Purpose:  new segment created with a number of words equal to r[c] and 
 *           each word initialized to zero. r[b] stores the new segment ID and
 *           the new segment is mapped to m[r[b]]
 */
static inline void map_seg(Seq_T unmap, int b, int c)
{
    assert(b >= 0 && b < 8);
    assert(c >= 0 && c < 8);
    assert(unmap != NULL);
    
    uint32_t words = reg[c];
    uint32_t val = map_segment(unmap, words);
    //fprintf(stderr, "val: %d\n", val);
    reg[b] = val;
}

/*
 * Function: unmap_seg
 * Input:    memory and register data structures, three ints 0-7
 * Return:   none
 * Purpose:  unmaps segment in m[r[c]]
 * Notes:    CRE if segment already unmapped
 */
static inline void unmap_seg(Seq_T unmap, int c)
{
    assert(c >= 0 && c < 8);
    assert(unmap != NULL);
    
    uint32_t id = reg[c];
    unmap_segment(unmap, id, false);
}

/*
 * Function: output
 * Input:    1 register to output
 * Return:   none
 * Purpose:  write value in reg c to stdout
 * Notes:    CRE if not ascii char
 */
static inline void output(int c)
{
    assert(c >= 0 && c < 8);
    
    uint32_t val = reg[c];
    assert(val < 256);
    putc(val, stdout);
}

/*
 * Function: input
 * Input:    register array, 1 int 0-7
 * Return:   none
 * Purpose:  waits for input on stdin, loads input into 32 bit word r[c] in
 *           which every bit is 1
 * Note:     CRE if not an ASCII character
 */
static inline void input(int c)
{
    assert(c >= 0 && c < 8);
    
    int val = getc(stdin);
    assert((val >= 0 && val < 256) || val == EOF);

    if (val == EOF) {
        reg[c] = ~0;
    }
    else {
        reg[c] = val;
    }
}

/*
 * Function: load_prog
 * Input:    memory and regsiter data structs, two registers 0-7
 * Return:   none
 * Purpose:  duplicate segment m[r[b]] and store it in segment 0.
 * Notes:    may fail if segment unmapped, should be quick if seg = 0
 */
static inline void load_prog(Seq_T unmap, int b)
{
    assert(b >= 0 && b < 8);
    assert(unmap != NULL);

    uint32_t id = reg[b];
    int seg_len = get_len(id);
    uint32_t val = 0;
    
    if (id != 0) {
        unmap_segment(unmap, 0, true);
        map_segment(unmap, seg_len);
        for (int i = 0; i < seg_len; i++) {
            val = get_val(id, i);
            set_val(0, i, val);
        }
    }
}

/*
 * Function: load_value
 * Input:    register array, int register id, uint32_t value
 * Return:   none
 * Purpose:  loads value into r[a]
 */
static inline void load_val(int a, uint32_t value)
{
    assert(a >= 0 && a < 8);
    
    reg[a] = value;
}

#endif
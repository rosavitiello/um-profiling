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

#ifndef INSTRUCTION_INCLUDED
#define INSTRUCTION_INCLUDED

/* ------------------------------- prototypes ------------------------------- */

/*
 * Function: conditional_move
 * Input:    register data structure, three ints 0-7
 * Return:   none
 * Purpose:  perform the following: if c != 0, a = b 
 */
void conditional_move(uint32_t *reg, int a, int b, int c);

/*
 * Function: seg_load 
 * Input:    memory and register array, three ints 0-7
 * Return:   none
 * Purpose:  loads value from segemented memory in mem[b][c] into reg[a]
 * Notes:    CRE if unmapped segment accessed
 */
void seg_load(Seq_T mem, uint32_t *reg, int a, int b, int c);

/*
 * Function: seg_store
 * Input:    memory and register data structures, three ints 0-7
 * Return:   none
 * Purpose:  stores value from reg[c] in mem[a][b]
 * Notes:    CRE if unmapped segment accessed
 */
void seg_store(Seq_T mem, uint32_t *reg, int a, int b, int c);

/*
 * Function: addition
 * Input:    register array, three ints 0-7
 * Return:   none
 * Purpose:  performs addition operation, r[a] = (r[b] + r[c]) mod 2^32
 */
void addition(uint32_t *reg, int a, int b, int c);

/*
 * Function: multiplication
 * Input:    register array, three ints 0-7
 * Return:   none
 * Purpose:  performs multiplication operation, r[a] = (r[b] x r[c]) mod 2^32
 */
void multiplication(uint32_t *reg, int a, int b, int c);

/*
 * Function: division
 * Input:    register data structure, three ints 0-7
 * Return:   none
 * Purpose:  performs r[a] = r[b]/r[c]
 * Notes:    CRE if divide by zero
 */
void division(uint32_t *reg, int a, int b, int c);

/*
 * Function: bitwise nand
 * Input:    register array, three ints 0-7
 * Return:   none
 * Purpose:  performs bitwise "not" + and operation, r[a] = ~(r[b] & r[c])
 */
void bitwise_nand(uint32_t *reg, int a, int b, int c);

/*
 * Function: halt
 * Input:    register and memory data structures
 * Return:   none
 * Purpose:  frees memory and halts program
 */
void halt(Seq_T mem, Seq_T unmap, uint32_t *reg);

/*
 * Function: map_seg
 * Input:    memory and register array, two ints 0-7
 * Return:   none
 * Purpose:  new segment created with a number of words equal to r[c] and 
 *           each word initialized to zero. r[b] stores the new segment ID and
 *           the new segment is mapped to m[r[b]]
 */
void map_seg(Seq_T mem, Seq_T unmap, uint32_t *reg, int b, int c);

/*
 * Function: unmap_seg
 * Input:    memory and register data structures, three ints 0-7
 * Return:   none
 * Purpose:  unmaps segment in m[r[c]]
 * Notes:    CRE if segment already unmapped
 */
void unmap_seg(Seq_T mem, Seq_T unmap, uint32_t *reg, int c);

/*
 * Function: output
 * Input:    1 register to output
 * Return:   none
 * Purpose:  write value in reg c to stdout
 */
void output(uint32_t *reg, int c);

/*
 * Function: input
 * Input:    register array, 1 int 0-7
 * Return:   none
 * Purpose:  waits for input on stdin, loads input into 32 bit word r[c] in
 *           which every bit is 1
 * Note:     CRE if not an ASCII character
 */
void input(uint32_t *reg, int c);

/*
 * Function: load_prog
 * Input:    memory and regsiter data structs, two registers 0-7
 * Return:   none
 * Purpose:  duplicate segment m[r[b]] and store it in segment 0.
 * Notes:    may fail if segment unmapped, should be quick if seg = 0
 */
void load_prog(Seq_T mem, Seq_T unmap, uint32_t *reg, int b);

/*
 * Function: load_value
 * Input:    register array, int register id, uint32_t value
 * Return:   none
 * Purpose:  loads value into r[a]
 */
void load_val(uint32_t *reg, int a, uint32_t value);

#endif
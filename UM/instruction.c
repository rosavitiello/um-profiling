/*
 * instruction.c
 * COMP 40 Fall 2019 um
 * 
 * instruction module implementation. Functions that perform each of the 14 RISC
 * operations.
 * 
 * Written by:    Rosa Vitiello, Ethan Schreiber
 * Last modified: Nov. 19, 2019
 */

/* -------------------------------- includes -------------------------------- */

#include "instruction.h"
#include "memory.h"

#include <assert.h>
#include <stdbool.h>

/* ------------------------- function definitions --------------------------- */

/*
 * Function: conditional_move
 * Input:    register data structure, three ints 0-7
 * Return:   none
 * Purpose:  perform the following: if c != 0, a = b 
 */
void conditional_move(uint32_t *reg, int a, int b, int c)
{
    assert(a >= 0 && a < 8);
    assert(b >= 0 && b < 8);
    assert(c >= 0 && c < 8);
    assert(reg != NULL);

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
void seg_load(Seq_T mem, uint32_t *reg, int a, int b, int c)
{
    assert(a >= 0 && a < 8);
    assert(b >= 0 && b < 8);
    assert(c >= 0 && c < 8);
    assert(mem != NULL);
    assert(reg != NULL);
    
    uint32_t val = get_val(mem, reg[b], reg[c]);
    reg[a] = val;
}

/*
 * Function: seg_store
 * Input:    memory and register data structures, three ints 0-7
 * Return:   none
 * Purpose:  stores value from reg[c] in mem[a][b]
 * Notes:    CRE if unmapped segment accessed
 */
void seg_store(Seq_T mem, uint32_t *reg, int a, int b, int c)
{
    assert(a >= 0 && a < 8);
    assert(b >= 0 && b < 8);
    assert(c >= 0 && c < 8);
    assert(mem != NULL);
    assert(reg != NULL);

    uint32_t val = reg[c];
    set_val(mem, reg[a], reg[b], val);
}

/*
 * Function: addition
 * Input:    register array, three ints 0-7
 * Return:   none
 * Purpose:  performs addition operation, r[a] = (r[b] + r[c]) mod 2^32
 */
void addition(uint32_t *reg, int a, int b, int c)
{
    assert(a >= 0 && a < 8);
    assert(b >= 0 && b < 8);
    assert(c >= 0 && c < 8);
    assert(reg != NULL);

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
void multiplication(uint32_t *reg, int a, int b, int c)
{
    assert(a >= 0 && a < 8);
    assert(b >= 0 && b < 8);
    assert(c >= 0 && c < 8);
    assert(reg != NULL);

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
void division(uint32_t *reg, int a, int b, int c)
{
    assert(a >= 0 && a < 8);
    assert(b >= 0 && b < 8);
    assert(c >= 0 && c < 8);
    assert(reg != NULL);
    
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
void bitwise_nand(uint32_t *reg, int a, int b, int c)
{
    assert(a >= 0 && a < 8);
    assert(b >= 0 && b < 8);
    assert(c >= 0 && c < 8);
    assert(reg != NULL);

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
void halt(Seq_T mem, Seq_T unmap, uint32_t *reg)
{
    assert(mem != NULL);
    assert(unmap != NULL);
    assert(reg != NULL);

    free_mem(mem, unmap);
    free(reg);
}

/*
 * Function: map_seg
 * Input:    memory and register array, two ints 0-7
 * Return:   none
 * Purpose:  new segment created with a number of words equal to r[c] and 
 *           each word initialized to zero. r[b] stores the new segment ID and
 *           the new segment is mapped to m[r[b]]
 */
void map_seg(Seq_T mem, Seq_T unmap, uint32_t *reg, int b, int c)
{
    assert(b >= 0 && b < 8);
    assert(c >= 0 && c < 8);
    assert(mem != NULL);
    assert(unmap != NULL);
    assert(reg != NULL);
    
    uint32_t words = reg[c];
    uint32_t val = map_segment(mem, unmap, words);
    reg[b] = val;
}

/*
 * Function: unmap_seg
 * Input:    memory and register data structures, three ints 0-7
 * Return:   none
 * Purpose:  unmaps segment in m[r[c]]
 * Notes:    CRE if segment already unmapped
 */
void unmap_seg(Seq_T mem, Seq_T unmap, uint32_t *reg, int c)
{
    assert(c >= 0 && c < 8);
    assert(mem != NULL);
    assert(unmap != NULL);
    assert(reg != NULL);
    
    uint32_t id = reg[c];
    unmap_segment(mem, unmap, id, false);
}

/*
 * Function: output
 * Input:    1 register to output
 * Return:   none
 * Purpose:  write value in reg c to stdout
 * Notes:    CRE if not ascii char
 */
void output(uint32_t *reg, int c)
{
    assert(c >= 0 && c < 8);
    assert(reg != NULL);
    
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
void input(uint32_t *reg, int c)
{
    assert(c >= 0 && c < 8);
    assert(reg != NULL);
    
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
void load_prog(Seq_T mem, Seq_T unmap, uint32_t *reg, int b)
{
    assert(b >= 0 && b < 8);
    assert(mem != NULL);
    assert(unmap != NULL);
    assert(reg != NULL);

    uint32_t id = reg[b];
    int len = get_len(mem, id);
    uint32_t val = 0;
    
    if (id != 0) {
        unmap_segment(mem, unmap, 0, true);
        map_segment(mem, unmap, len);
        for (int i = 0; i < len; i++) {
            val = get_val(mem, id, i);
            set_val(mem, 0, i, val);
        }
    }
}

/*
 * Function: load_value
 * Input:    register array, int register id, uint32_t value
 * Return:   none
 * Purpose:  loads value into r[a]
 */
void load_val(uint32_t *reg, int a, uint32_t value)
{
    assert(a >= 0 && a < 8);
    assert(reg != NULL);
    
    reg[a] = value;
}
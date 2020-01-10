/*
 * operation.c
 * COMP 40 Fall 2019 um
 * 
 * Implementation for operation function. Initializes UM memory and runs UM
 * with program counter
 * 
 * Written by:    Rosa Vitiello, Ethan Schreiber
 * Last modified: Nov. 17, 2019
 */

/* -------------------------------- includes -------------------------------- */

#include "operation.h"
#include "memory.h"
#include "instruction.h"

#include <seq.h>
#include <bitpack.h>
#include <assert.h>

/* ------------------------------ definitions ------------------------------- */

/* constants */
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

/* opcodes for instruction */
enum opcode {COND_MOVE, SEG_LOAD, SEG_STORE, ADD, MULT, DIV, NAND, HALT, MAP, 
             UNMAP, OUTPUT, INPUT, LOAD_PROG, LOAD_VAL};

/* holds two sequences for memory */
typedef struct UM_mem {
    Seq_T mem;
    Seq_T unmap;
} UM_mem;


/* ------------------------- function definitions --------------------------- */

/*
 * Function: init
 * Input:    file * fp
 * Return:   Seq_T with init 0
 * Purpose:  read instructions from file and store in seg 0
 */
UM_mem init(FILE *fp)
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
    Seq_T mem = new_mem();
    Seq_T unmap = Seq_new(1);

    /* copy from array to memory */
    int len = Seq_length(instr);
    map_segment(mem, unmap, len);
    for (int i = 0; i < len; i++) {
        uint32_t *val = (uint32_t *)Seq_remlo(instr);
        set_val(mem, 0, i, *val);
        free(val);
    }

    /* create memory struct for returning */
    UM_mem memory;
    memory.mem = mem;
    memory.unmap = unmap; 

    free(word);
    Seq_free(&instr);
    return memory;
}

/*
 * Function: run
 * Input:    file * fp
 * Return:   none
 * Purpose:  initializes and runs um!
 */
void run(FILE *fp)
{
    uint32_t * reg = (uint32_t *)calloc(8, sizeof(uint32_t));
    assert(reg != NULL);
    assert(fp != NULL);

    int prog_ctr = 0;
    UM_mem memory = init(fp);
    int code = 0;
    int a;
    int b;
    int c;
    int val;
    
    while (code != HALT) {
        /* get instruction from memory*/
        uint32_t instr = get_val(memory.mem, 0, prog_ctr);
        
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
            case COND_MOVE:
                conditional_move(reg, a, b, c);
                break;
            case SEG_LOAD:
                seg_load(memory.mem, reg, a, b, c);
                break;
            case SEG_STORE:
                seg_store(memory.mem, reg, a, b, c);
                break;
            case ADD:
                addition(reg, a, b, c);
                break;
            case MULT:
                multiplication(reg, a, b, c);
                break;
            case DIV:
                division(reg, a, b, c);
                break;
            case NAND:
                bitwise_nand(reg, a, b, c);
                break;
            case HALT:
                halt(memory.mem, memory.unmap, reg);
                break;
            case MAP:
                map_seg(memory.mem, memory.unmap, reg, b, c);
                break;
            case UNMAP:
                unmap_seg(memory.mem, memory.unmap, reg, c);
                break;
            case OUTPUT:
                output(reg, c);
                break;
            case INPUT:
                input(reg, c);
                break;
            case LOAD_PROG:
                prog_ctr = reg[c]; /* set prog ctr to reg c */
                load_prog(memory.mem, memory.unmap, reg, b);
                break;
            case LOAD_VAL:
                load_val(reg, a, val);
                break;
        }
    }
}
/*
 * memory.h
 * COMP 40 Fall 2019 um
 * 
 * Header file for segmented memory
 * 
 * Written by:    Rosa Vitiello, Ethan Schreiber
 * Last modified: Nov. 14, 2019
 */

/* -------------------------------- includes -------------------------------- */

#include "seq.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifndef MEMORY_INCLUDED
#define MEMORY_INCLUDED

/* ------------------------------- prototypes ------------------------------- */


/* ------------------------------- global -------------------------------- */

int mem_size = 1;
int memory_len = 0;

/* ------------------------------ definitions ------------------------------- */

/* Segment memory struct */
typedef struct Segment {
    uint32_t *arr;
    int len;
} Segment;

Segment *seq = NULL;

/* ------------------------- function definitions --------------------------- */

/*
 * Function: new_mem
 * Input:    none
 * Return:   Seq_T mem
 * Purpose:  initalizes and returns segment memory in a Seq_T
 */
static inline void new_mem() {
    seq = calloc(mem_size, sizeof(Segment));
    /* 1 because only 1 guaranteed segment (0) */
}

/*
 * Function: map_segment
 * Input:    valid Seq_T, can have any number of mapped or unmapped segments
 *           int representing number of words in new segment
 * Return:   int representing num of new mapped segment
 * Purpose:  map a new segment of 'words' words into the sequence
 */
static inline int map_segment(Seq_T unmap, int words)
{
    /* CRE if bad user input */
    assert(seq != NULL);
    assert(words > 0);
    
    /* create new array */
    uint32_t *arr = calloc(words, sizeof(uint32_t));
    assert(arr != NULL);

    /* create new segment */
    Segment seg;
    seg.arr = arr;
    seg.len = words;
    
    /* get correct index and map */

    int index = -1;
    if (Seq_length(unmap) != 0){
        int *unmap_id = Seq_remlo(unmap);
        index = *unmap_id;
        free(unmap_id);
    }
    
    /* map new memory to correct index */
    if (index == -1) { /* no reuse */
        if (memory_len >= mem_size) {
            /* recalloc */
            Segment * temp = seq;
            //fprintf(stderr, "seq %p\n", (void *)seq);
            seq = calloc(mem_size * 2, sizeof(Segment));
            mem_size *= 2;
            for (int i = 0; i < memory_len; i++) {
                seq[i].arr = temp[i].arr;
                seq[i].len = temp[i].len;
            }
            free(temp);
            //fprintf(stderr, "seq %p\n", (void *)seq);
            /*assign*/
            seq[memory_len] = seg;
            index = memory_len;
            memory_len++;
        }
        else {
            seq[memory_len] = seg;
            index = memory_len;
            memory_len++;
        }
    }
    else {
        seq[index] = seg;
    }
    return index;
}

/*
 * Function: unmap_segment
 * Input:    valid Seq_T, int giving id to be unmapped
 * Return:   none
 * Purpose:  unmap the given segment, or return false if unmap fails
 * Notes:    CRE if segment already not mapped, or if segment is 0
 */
static inline void unmap_segment(Seq_T unmap, int id, bool load)
{
    assert(seq != NULL);

    /* ensure valid id */
    if (!load){
        assert(id > 0);
    }
    Segment seg = seq[id];
    assert(seg.arr != NULL);

    /* unmap */
    free(seg.arr);
    seq[id].arr = NULL;

    int *unmap_id = malloc(sizeof(int));
    assert(unmap_id != NULL);
    *unmap_id = id;
    Seq_addlo(unmap, unmap_id);
}

/*
 * Function: get_val
 * Input:    Seq_T seq, int seq ID, int offset in segment
 * Return:   uint32_t value at m[id][offset]
 * Purpose:  gets value in segmented memory at m[id][offset]
 * Notes:    CRE if m[id][offset] is not in memory (out of bounds)
 */
static inline uint32_t get_val(int id, int offset)
{
    /* CRE bad input */
    assert(seq != NULL);
    assert(id >= 0 && offset >= 0);

    /* check len of segment */
    Segment seg = seq[id];
    assert(seg.arr != NULL);
    int len = seg.len;
    assert(offset < len);

    /* get the value */
    return seg.arr[offset];
}

/*
 * Function: set_val
 * Input:    Seq_T seq, int seq ID, int offset in segment, uint32_t val to set
 * Return:   none
 * Purpose:  sets value in segmented memory at m[id][offset]
 * Notes:    CRE if m[id][offset] is not in memory (out of bounds)
 */
static inline void set_val(int id, int offset, uint32_t val) 
{
    /* CRE bad user input */
    assert(seq != NULL);
    assert(id >= 0 && offset >= 0);
    
    /* check len of segment */
    Segment seg = seq[id];
    assert(seg.arr != NULL);
    int len = seg.len;  
    assert(offset < len);
    
    /* set the value */
    seg.arr[offset] = val;
}

/*
 * Function: get_len
 * Input:    Seq_T seq, int seq ID
 * Return:   none
 * Purpose:  gets number of words in segment
 */
static inline int get_len(int id)
{
    assert(seq != NULL);

    Segment seg = seq[id];
    assert(seg.arr != NULL);
    return seg.len;
}

/*
 * Function: free_mem
 * Input:    Seq_T *seq
 * Return:   none
 * Purpose:  frees memory held in segemented memory
 */
static inline void free_mem(Seq_T unmap)
{
    assert(unmap != NULL);
    assert(seq != NULL);

    /* free memory sequence */
    for (int i = 0; i < memory_len; i++) {
        Segment seg = seq[i];
        if (seg.arr != NULL) {
            free(seg.arr);
        }
    }

    /* free unmap sequence */
    int un_len = Seq_length(unmap);
    for (int i = 0; i < un_len; i++) {
        int *unmap_id = Seq_remlo(unmap);
        free(unmap_id);
    }

    Seq_free(&unmap);
    free(seq);
}


#endif

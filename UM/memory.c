/*
 * memory.c
 * COMP 40 Fall 2019 um
 * 
 * Implementation for segmented memory of um
 * 
 * Written by:    Rosa Vitiello, Ethan Schreiber
 * Last modified: Nov. 14, 2019
 */

/* -------------------------------- includes -------------------------------- */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "memory.h"

/* ------------------------------ definitions ------------------------------- */

/* Segment memory struct */
typedef struct Segment {
    uint32_t *arr;
    int len;
} Segment;

/* ------------------------- function definitions --------------------------- */

/*
 * Function: new_mem
 * Input:    none
 * Return:   Seq_T mem
 * Purpose:  initalizes and returns segment memory in a Seq_T
 */
Seq_T new_mem() {
    return Seq_new(1); 
}

/*
 * Function: map_segment
 * Input:    valid Seq_T, can have any number of mapped or unmapped segments
 *           int representing number of words in new segment
 * Return:   int representing num of new mapped segment
 * Purpose:  map a new segment of 'words' words into the sequence
 */
int map_segment(Seq_T seq, Seq_T unmap, int words)
{
    /* CRE if bad user input */
    assert(seq != NULL);
    assert(words > 0);
    
    /* create new array */
    uint32_t *arr = calloc(words, sizeof(uint32_t));
    assert(arr != NULL);

    /* create new segment */
    Segment *seg = malloc(sizeof(Segment));
    assert(seg != NULL);
    seg -> arr = arr;
    seg -> len = words;
    
    /* get correct index and map */
    int index = -1;
    if (Seq_length(unmap) != 0){
        int *unmap_id = Seq_remlo(unmap);
        index = *unmap_id;
        free(unmap_id);
    }
    
    /* map new memory to correct index */
    if (index == -1) { /* no reuse case */
        Seq_addhi(seq, seg);
        index = Seq_length(seq) - 1;
    }
    else {
        Seq_put(seq, index, seg);
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
void unmap_segment(Seq_T seq, Seq_T unmap, int id, bool load)
{
    assert(seq != NULL);

    /* ensure valid id */
    if (!load){
        assert(id > 0);
    }
    Segment *seg = Seq_get(seq, id);
    assert(seg != NULL);

    /* unmap */
    free(seg -> arr);
    free(seg);
    Seq_put(seq, id, NULL);

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
uint32_t get_val(Seq_T seq, int id, int offset)
{
    /* CRE bad input */
    assert(seq != NULL);
    assert(id >= 0 && offset >= 0);

    /* check len of segment */
    Segment *seg = Seq_get(seq, id);
    assert(seg != NULL);
    int len = seg -> len;
    assert(offset < len);

    /* get the value */
    return seg -> arr[offset];
}

/*
 * Function: set_val
 * Input:    Seq_T seq, int seq ID, int offset in segment, uint32_t val to set
 * Return:   none
 * Purpose:  sets value in segmented memory at m[id][offset]
 * Notes:    CRE if m[id][offset] is not in memory (out of bounds)
 */
void set_val(Seq_T seq, int id, int offset, uint32_t val) 
{
    /* CRE bad user input */
    assert(seq != NULL);
    assert(id >= 0 && offset >= 0);
    
    /* check len of segment */
    Segment *seg = Seq_get(seq, id);
    assert(seg != NULL);
    int len = seg -> len;  
    assert(offset < len);
    
    /* set the value */
    seg -> arr[offset] = val;
}

/*
 * Function: get_len
 * Input:    Seq_T seq, int seq ID
 * Return:   none
 * Purpose:  gets number of words in segment
 */
int get_len(Seq_T seq, int id)
{
    assert(seq != NULL);

    Segment *seg = Seq_get(seq, id);
    assert(seg != NULL);
    return seg -> len;
}

/*
 * Function: free_mem
 * Input:    Seq_T *seq
 * Return:   none
 * Purpose:  frees memory held in segemented memory
 */
void free_mem(Seq_T seq, Seq_T unmap)
{
    assert(unmap != NULL);
    assert(seq != NULL);

    /* free memory sequence */
    int len = Seq_length(seq);
    for (int i = 0; i < len; i++) {
        Segment *seg = Seq_remlo(seq);
        if (seg != NULL) {
            free(seg -> arr);
            free(seg);
        }
    }

    /* free unmap sequence */
    len = Seq_length(unmap);
    for (int i = 0; i < len; i++) {
        int *unmap_id = Seq_remlo(unmap);
        free(unmap_id);
    }

    Seq_free(&unmap);
    Seq_free(&seq);
}

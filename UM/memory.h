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

#ifndef MEMORY_INCLUDED
#define MEMORY_INCLUDED

/* ------------------------------- prototypes ------------------------------- */

/*
 * Function: new_mem
 * Input:    none
 * Return:   Seq_T mem
 * Purpose:  initalizes and returns segment memory in a Seq_T
 */
Seq_T new_mem();

/*
 * Function: map_segment
 * Input:    valid Seq_T, can have any number of mapped or unmapped segments
 *           int representing number of words in new segment
 * Return:   int representing num of new mapped segment
 * Purpose:  map a new segment of 'words' words into the sequence
 */
int map_segment(Seq_T seq, Seq_T unmap, int words);

/*
 * Function: unmap_segment
 * Input:    valid Seq_T, int giving id to be unmapped
 * Return:   none
 * Purpose:  unmap the given segment, or return false if unmap fails, boolean if
 *           program is loaded or mapped
 * Notes:    CRE if segment already not mapped, or if segment is 0
 */
void unmap_segment(Seq_T seq, Seq_T unmap, int id, bool load);

/*
 * Function: get_val
 * Input:    Seq_T seq, int seq ID, int offset in segment
 * Return:   uint32_t value at m[id][offset]
 * Purpose:  gets value in segmented memory at m[id][offset]
 * Notes:    CRE if m[id][offset] is not in memory (out of bounds)
 */
uint32_t get_val(Seq_T seq, int id, int offset);

/*
 * Function: get_val
 * Input:    Seq_T seq, int seq ID, int offset in segment, uint32_t val to set
 * Return:   none
 * Purpose:  sets value in segmented memory at m[id][offset]
 * Notes:    CRE if m[id][offset] is not in memory (out of bounds)
 */
void set_val(Seq_T seq, int id, int offset, uint32_t val);

/*
 * Function: get_len
 * Input:    Seq_T seq, int seq ID
 * Return:   none
 * Purpose:  gets number of words in segment
 */
int get_len(Seq_T seq, int id);

/*
 * Function: free_mem
 * Input:    Seq_T *seq
 * Return:   none
 * Purpose:  frees memory held in segemented memory
 */
void free_mem(Seq_T seq, Seq_T unmap);

#endif

#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include "../tcl/tcl.h"
#define CYN  "\x1B[36m"		// Cyan Color //
#define NRM  "\x1B[0m"		// Normal Color //
//#define INFO 1
// Lookup table record structure //
// Contains complete information about every access on memory	//

typedef struct lut_record {
  unsigned int idx;                                   // Lookup table index
  char addr[10];                                      // Memory address
  int m_size;                                         // Memory size
  unsigned int thread_id;                             // Thread id
  char var_name[100];                                  // Variable name
  char func_name[1024];                                 // Function name
  char scope;                                         // Variable scope (G/S/H/M)
  char var_type;                                      // 2nd variable scope (G/L/H)
  char var_type2;                                     // Variable type (S/V)
  int heap_level;                                     // Heap level
  int line_num;                                       // Line number (allocation of dynamic memory)
  unsigned int var_offset;                            // Variable offset
  int array_fl;                                       // Array flag
} lut_record;

// Memory trace line structure //
// Used to save every line of gleipnir output	//
// Gleipnir tracks every access on memory providing the following information	//
typedef struct record_line {
  char operation;                                     // Operation
  char address[10];                                   // Memory address
  int mem_size;                                       // Memory size
  unsigned int thread_id;                             // Thread id
  char var_scope;                                     // Variable scope (G/S/H/M)
  char shoname[1024];                                   // Shared object name
  char fn_nm[1024];                                     // Function name
  char var_scope2;                                    // 2nd variable scope (G/L/H)
  char var_type;                                      // Variable type (S/V)
  char filename[1024];                                  // Filename (dynamic memory)
  int h_lvl;                                          // Heap level
  int ln_n;                                           // Line number (allocation of dynamic memory)
  unsigned int var_off;                               // Variable offset
  char var_name[100];                                  // Variable name
  char member[100];                                    // Member name (struct)
  int invalid_line;                                   // Invalid line flag
  int array_fl;                                       // Array flag
} record_line;

// Output pair structure //
// Output pair refers to all pairs of accesses of Operation and Index	//
typedef struct output_pair {
  char operation;                                     // Operation
  int index;                                          // Lookup table index
} output_pair;

typedef struct rd_list_node {
	unsigned int index;
	unsigned int number_of_times;
} rd_list_node;

/* Global variables */
unsigned int global_record_idx;	// Index for every access we record on our look up table	//
unsigned int pair_cnt;	// Counter of how many pairs we have //
int zero_match_fl;
output_pair *pair_array;	// Array containing the struct of pair of accesses with Operation and Index	//
int struct_fl;

//Added by Pavlos Aimoniotis 11/10/2019. Used to keep information about Heap in order to create a lut record. //
//Probably it will be unused.	//
int same_heap_address, heap_idx; // Used to identify specifix information on heap	//
//unsigned int heap_var_offset=0;

typedef struct  heap_accessed_info{
	char heap_address[10], heap_var_name[10]; // Struct to store every access on heap	//
} heap_accessed_info;

heap_accessed_info heap[20]; //an array with all heap variables saved

char completepath[2000]; // path to source directory. We need because src dir may be in different place than executable
char executablepath[2000];
char valgrindpath[2000];
char arguments[2000];
char stackorheap[2];
int foundpath;
//Addition end

#endif

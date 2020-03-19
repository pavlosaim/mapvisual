/* Memory Access Pattern Visualization */
// Author: Pavlos Aimoniotis
////////////////////////////////////////
// Headers of almost all filtering_functions.c

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "structs.h"

void free_lut(lut_record **);
void write_to_lut_file(lut_record *, FILE *); // Function to write Look Up Table information on a file	//
void print_lut_record(lut_record *); // Dump function - printing	//
void detect_allocated_info(FILE *); // Used to parser gleipnir trace file, detect starting address of a variable, bytes and type (int, double etc.)

int check_lut_table(lut_record **, record_line); // Function to check the look up table for any duplicate entries	//

char *get_dyn_var_name(char *, int);	// Function used to get variable name of source.c:line on heap detection	//
char *readable_fs(double, char *);

record_line validate_line(char *); // Function which parses Gleipnir Lines	//

output_pair * add_to_array(output_pair *, char, unsigned int); // Function used to identify every pair and add it on our array of all the pairs	//

lut_record ** add_lut_record(lut_record **, lut_record); // Function to add a Gleipnir Tool Parsed line on our Look Up Table	//
lut_record ** check_line(char *, lut_record **, FILE *);
lut_record ** create_lut_record(lut_record **, record_line, FILE *); // Function to create look up table record	//

#endif

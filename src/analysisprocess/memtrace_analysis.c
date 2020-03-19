/* Memory Access Pattern Visualization */
// Author: Pavlos Aimoniotis
////////////////////////////////////////
// This file parses the code from memory tracing
// output gleipnir format. Uses functions from 
// filter folder.

#include "../libs/functions.h"
#include "../libs/structs.h"

int main(int argc, char **argv) {

lut_record **lut;
lut=NULL;
long int tracefile_length;
char line_buffer[1024],buf[10],buffer[1024];
unsigned int file_lut_length,tmp_file_lut_length,savings,i;
double f_res;
output_pair read_pair;
FILE *access_list_file, *file_lut, *lut_file = NULL, *access_list = NULL, *tracefile, *addition_info_file;

tracefile = fopen(argv[1], "r");  
if(tracefile == NULL) {
	printf("There was an error opening the tracefile!\n");
	return 1;
}
detect_allocated_info(tracefile);
lut_file = fopen("lut_file","w+b");
access_list = fopen("access_list","w+b");
if ((lut_file == NULL) || (access_list == NULL)){
	printf( "Could not open file lut_file or access_list" ) ;
	return 1;
}

// Parse gleipnir tool line by line	//
// It also on check_line it creates all pairs of accesses with Operation and Index	//
while (fgets(line_buffer, sizeof(line_buffer), tracefile) != NULL) {
	lut=check_line(line_buffer,lut,lut_file);
}
// Store all pairs on access_list file	//
for(i = 0; i < pair_cnt; i++) {
	fwrite(&pair_array[i], sizeof(output_pair), 1, access_list);
}

fclose(access_list);
fclose(lut_file);

access_list_file = fopen("access_list","rb");
file_lut = fopen("lut_file","rb");

read_pair.operation = 0;
read_pair.index = 0;
while(!feof(access_list_file)){
	fread(&read_pair, sizeof(output_pair), 1, access_list_file);
}

fseek(tracefile, 0, SEEK_END);
tracefile_length = ftell(tracefile);
fclose(tracefile);

// Tracking information from parsed file
addition_info_file = fopen("additional_info.txt","w");
fprintf(addition_info_file, "Gleipnir memory trace filesize:  %s \n", readable_fs(tracefile_length, buf));
fprintf(addition_info_file, "\n");

tmp_file_lut_length = 0;
fseek(file_lut, 0L, SEEK_END);
file_lut_length = ftell(file_lut);
fprintf(addition_info_file, "Number of unique lut records:    %d\n",global_record_idx);
fprintf(addition_info_file, "lut filesize:                    %s \n",readable_fs(file_lut_length, buf));
tmp_file_lut_length = tmp_file_lut_length+file_lut_length;
fseek(access_list_file, 0L, SEEK_END);
file_lut_length = ftell(access_list_file);
fprintf(addition_info_file, "Memory accesses list filesize:   %s \n",readable_fs(file_lut_length, buf)); 
printf("\n");
tmp_file_lut_length = tmp_file_lut_length + file_lut_length;
if(tmp_file_lut_length < tracefile_length) {
	savings = tracefile_length - tmp_file_lut_length;
	f_res=((double)savings / (double)tracefile_length)*100;
	fprintf(addition_info_file, "Size reduced by:                 %s  ~ %2.2lf%% of the original file size\n",readable_fs(savings,buf),f_res);
} 
else {
	savings = tmp_file_lut_length - tracefile_length;
	f_res = ((double)savings / (double)tracefile_length)*100;
	fprintf(addition_info_file, "Size increased by:               %s  ~ %2.2lf%% of the original file size\n",readable_fs(savings,buf),f_res);
}

fclose(file_lut);
fclose(access_list_file);
fclose(addition_info_file);
free_lut(lut);
free(pair_array);
return 0;
}

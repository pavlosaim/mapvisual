/* Memory Access Pattern Visualization */
// Author: Pavlos Aimoniotis
////////////////////////////////////////
// In this file we try to collect data from 
// memory tracking output file. Basically, 
// the parsed file is scanned and important
// information is classified in files.

#include "../libs/functions.h"

int main(int argc, char **argv) {

lut_record **lut;
unsigned int i;
output_pair read_pair;
FILE *access_file, *lut_file, *additional_info_file, *lut_stats_out, *order_out;
char mem_alloc[2];

strcpy(mem_alloc, argv[1]); // get variable from tcl command, stack or heap?

access_file=fopen("access_list","rb");
lut_file=fopen("lut_file","rb");
additional_info_file=fopen("additional_info.txt","a");

lut=NULL;

read_pair.operation=0;
read_pair.index=0;	

// Storing the access_list to a dynamic output_array struct
// access_list has been created from memory_trace
while(!feof(access_file)) {
	fread(&read_pair,sizeof(output_pair),1,access_file);	  
	pair_array=add_to_array(pair_array,read_pair.operation,read_pair.index);
}
// Storing the lookup table to a dynamic lut_record array
lut_record tmp_record;
while(!feof(lut_file)) {
	fread(&tmp_record,sizeof(lut_record),1,lut_file);
	lut=add_lut_record(lut,tmp_record);
}

fprintf(additional_info_file,"Total memory accesses:				 %d\n",pair_cnt);
fprintf(additional_info_file,"\n");

fclose(additional_info_file);

fclose(lut_file);
fclose(access_file);


unsigned int appearance_cnt[global_record_idx];		// Appearance counter for every record
unsigned int order_of_access[pair_cnt];				// Order of accesses
unsigned int pos;									// Matching index of pairs to lut records
unsigned int mem_usage_s[global_record_idx];		// Stored bytes
unsigned int mem_usage_l[global_record_idx];		// Loaded bytes

for(i=0;i<global_record_idx;i++) {
	appearance_cnt[i]=0;
	mem_usage_l[i]=0;
	mem_usage_s[i]=0;
}

for(i=0;i<pair_cnt;i++) {

	//Get info for accesses
	pos=pair_array[i].index;
	appearance_cnt[pos]++;
	order_of_access[i]=pair_array[i].index;

	//Count total size of stores - loads
	switch(pair_array[i].operation) {
		case 'S':
			mem_usage_s[pos]+=lut[pos]->m_size;
			break;
		case 'L':
			mem_usage_l[pos]+=lut[pos]->m_size;
			break;
		case 'M':
			mem_usage_l[pos]+=lut[pos]->m_size; 
			mem_usage_s[pos]+=lut[pos]->m_size;
			break;
	}

}


lut_stats_out=fopen("memory_stats","w");

/* Heatmap data (index , number of appearances , address , variable name) */
fprintf(lut_stats_out, "Index\tAppearance Count\tAddress\t  Memory Usage Store\tMemory Usage Load\tVariable name\n");
for(i=0;i<global_record_idx;i++) {
	fprintf(lut_stats_out,"%d\t\t%d\t\t%s\t\t%d\t\t%d\t\t%s\n",i,appearance_cnt[i],lut[i]->addr,mem_usage_s[i],mem_usage_l[i],lut[i]->var_name);
	#ifdef INFO
		printf("Index: %d, Appearance Count: %d, Address: %s, Memory Usage Store: %d, Memory Usage Load: %d, Variable name: %s\n",i,appearance_cnt[i],lut[i]->addr,mem_usage_s[i],mem_usage_l[i],lut[i]->var_name);
	#endif
}	

fclose(lut_stats_out);


order_out=fopen("order_data.txt","w");
/* Order data (order of appearance , address , variable name) */
for(i=0;i<pair_cnt;i++) {
		// By default 1 stands for Stack, 2 stands for Heap
		if (strcmp(mem_alloc, "H")==0){
			if (lut[pair_array[i].index]->scope == 'H')
				fprintf(order_out,"%d %s %c %s\n",order_of_access[i],lut[pair_array[i].index]->addr,pair_array[i].operation,lut[pair_array[i].index]->var_name);
				#ifdef INFO
					printf("Order of Access: %d, Address: %s, Operation: %c, Variable name: %s\n",order_of_access[i],lut[pair_array[i].index]->addr,pair_array[i].operation,lut[pair_array[i].index]->var_name);
				#endif		
		}
		else{
			if (lut[pair_array[i].index]->scope == 'S')
				fprintf(order_out,"%d %s %c %s\n",order_of_access[i],lut[pair_array[i].index]->addr,pair_array[i].operation,lut[pair_array[i].index]->var_name);
				#ifdef INFO
					printf("Order of Access: %d, Address: %s, Operation: %c, Variable name: %s\n",order_of_access[i],lut[pair_array[i].index]->addr,pair_array[i].operation,lut[pair_array[i].index]->var_name);
				#endif		
		}
}

fclose(order_out);
free_lut(lut);
free(pair_array);
return 0;

}

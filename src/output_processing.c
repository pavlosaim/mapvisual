#include "structs.h"
#include "functions.h"

// In this file processing for the output files of memtrace is happening.	//
// Mainly creates files that are used by map_analysis.py to create the output data	//
// Like Variable Metrics, Memory Usage etc. //
// Most of the variables are counted in map_analysis.py but the files that are parsed there are created here	//
int main(int argc, char **argv) {

	lut_record **lut;
 	lut=NULL;
 	unsigned int i;
 	output_pair read_pair;
	char stackorheap; 


	stackorheap = *(argv[1]);// By default S stands for Stack, H stands for Heap

 	read_pair.operation=0;
	read_pair.index=0;	

	FILE* access_file;
	FILE* lut_file;
	FILE* additional_info_file;

	access_file=fopen("access_list","rb");
	lut_file=fopen("lut_file","rb");
	additional_info_file=fopen("additional_info.txt","a");

	/* Storing the access_list to a dynamic output_array struct */
	while(!feof(access_file)) {
	  fread(&read_pair,sizeof(output_pair),1,access_file);	  
	  pair_array=add_to_array(pair_array,read_pair.operation,read_pair.index);
	}
	/* Storing the lookup table to a dynamic lut_record array */
	lut_record tmp_record;
	while(!feof(lut_file)) {
	  fread(&tmp_record,sizeof(lut_record),1,lut_file);
	  lut=add_lut_record(lut,tmp_record);
	}

	fprintf(additional_info_file,"Total memory accesses:				 %d\n",pair_cnt);
	fprintf(additional_info_file,"\n");

	fclose(lut_file);
	fclose(access_file);

	/* Initialization */
	unsigned int appearance_cnt[global_record_idx];		// Appearance counter for every record
	unsigned int order_of_access[pair_cnt];				// Order of accesses
	unsigned int pos;									// Matching index of pairs to lut records
	unsigned int last_appearance[global_record_idx];	// Last appearance of record
	unsigned int new_rd;								// New reuse distance

	rd_list_node * rd_bins;
	unsigned int rd_bins_num=1;							// Max reuse distance
	unsigned int mem_usage_s[global_record_idx];		// Stored bytes
	unsigned int mem_usage_l[global_record_idx];		// Loaded bytes
	unsigned int rd_sum=0;
	unsigned int sq_sum=0;								// Cumulative sum of each (reuse distance - average reuse distance)^2
	float avg_rd[pair_cnt];								// Average reuse distance
	float var_rd[pair_cnt];								// Reuse distance variance
	int rd_exists_fl=0;
	double std_dev;
	int tmp_var=0;

	rd_bins=malloc(rd_bins_num*sizeof(rd_list_node));
	rd_bins[0].index=0;
	rd_bins[0].number_of_times=0;

	for(i=0;i<global_record_idx;i++) {
		appearance_cnt[i]=0;
		last_appearance[i]=0;
		mem_usage_l[i]=0;
		mem_usage_s[i]=0;
	}
	for(i=0;i<pair_cnt;i++) {
		avg_rd[i]=0;
		var_rd[i]=0;
	}

	for(i=0;i<pair_cnt;i++) {
		pos=pair_array[i].index;
		appearance_cnt[pos]++;
		order_of_access[i]=pair_array[i].index;
		new_rd=0;

		switch(pair_array[i].operation) {
			case 'S':
				mem_usage_s[pos]+=lut[pos]->m_size;
				break;
			case 'L':
				mem_usage_l[pos]+=lut[pos]->m_size;
				break;
			case 'M':
				mem_usage_l[pos]+=lut[pos]->m_size; //Changed by Pavlos Aimoniotis 11/10/2019, previously was mem_usage_s[pos]+=lut[pos]->m_size
				mem_usage_s[pos]+=lut[pos]->m_size;
				break;
		}

		if(last_appearance[pos]>0) {
			new_rd=i-last_appearance[pos];
			rd_sum+=new_rd;
			avg_rd[i]=rd_sum/i;
			sq_sum=(new_rd-avg_rd[i-1])*(new_rd-avg_rd[i-1]);
			tmp_var=((float)1/(float)i)*(float)sq_sum;		
			std_dev=sqrt(tmp_var);
			var_rd[i]=std_dev;

			for(int k=0;k<rd_bins_num;k++) {		// If the reuse distance already exists on the list
				if(rd_bins[k].index == new_rd) {	// increase the number of times.
					rd_bins[k].number_of_times++;
					rd_exists_fl=1;
				}
			}

			if(!rd_exists_fl) {
				rd_bins_num++;
				rd_bins=realloc(rd_bins,rd_bins_num*sizeof(rd_list_node));
				rd_bins[rd_bins_num-1].index=new_rd;
				rd_bins[rd_bins_num-1].number_of_times=1;
			}
			rd_exists_fl=0;
			last_appearance[pos]=i;
		 }
		else {
			last_appearance[pos]=i;
			if(i>0) {
				var_rd[i]=var_rd[i-1];
				avg_rd[i]=rd_sum/i;
			}
		}


	}
	// This functionality can be added later on. It gives the opportunity on the user to decide if he wants to check either for stack or heap allocation	//
	// Default status is heap allocation	//
	
	//printf("Would you like to check stack (S) or heap (H) allocations?\n");
	//scanf("%c", &stackorheap);
	//while (stackorheap!= 'S' && stackorheap!='H'){
	//	printf("Enter S for stack, H for heap.\n");
	//	scanf("%c", &stackorheap);
	//}

	FILE* stats_out=fopen("stats.txt","w");
	FILE* scatter_out=fopen("scatterplot_data.txt","w");
	FILE* lut_stats_out=fopen("lut_stats_data.txt","w");
	FILE* order_out=fopen("order_data.txt","w");
	/* Statistical data after each access (average reuse distance , variance) */
	for(i=1;i<pair_cnt;i++) {
		fprintf(stats_out,"%f %f\n",avg_rd[i],var_rd[i]);
	}

	/* Scatterplot data (reuse distance , number of times) */
	for(i=1;i<rd_bins_num;i++) {
		if(rd_bins[i].number_of_times>2) {
			fprintf(scatter_out,"%d %d\n",rd_bins[i].index,rd_bins[i].number_of_times); 
		}
	}	
	/* Heatmap data (index , number of appearances , address , variable name) */
	for(i=0;i<global_record_idx;i++) {
		fprintf(lut_stats_out,"%d %d %s %d %d %s\n",i,appearance_cnt[i],lut[i]->addr,mem_usage_s[i],mem_usage_l[i],lut[i]->var_name);
		#ifdef INFO
			printf("Index: %d, Appearance Count: %d, Address: %s, Memory Usage Store: %d, Memory Usage Load: %d, Variable name: %s\n",i,appearance_cnt[i],lut[i]->addr,mem_usage_s[i],mem_usage_l[i],lut[i]->var_name);
		#endif
	}	
	/* Order data (order of appearance , address , variable name) */
	for(i=0;i<pair_cnt;i++) {
			// By default 1 stands for Stack, 2 stands for Heap
			if (stackorheap == 'H'){
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

	fclose(scatter_out);
	fclose(lut_stats_out);
	fclose(order_out);
	fclose(stats_out);
	fclose(additional_info_file);
	/* Freeing memory */
	free_lut(lut);
	free(pair_array);
	free(rd_bins);
	return 0;
	
}

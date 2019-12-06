#include "structs.h"

int foundpath=0;
// Add a pair of operation and index on array of struct //
output_pair * add_to_array(output_pair *array, char op, unsigned int idx) {

  if (pair_cnt > 0) {
    pair_cnt++;
    array = realloc(array, pair_cnt*sizeof(output_pair));
    array[pair_cnt-1].operation = op;
    array[pair_cnt-1].index = idx;
  } 
	else {
    array = malloc(sizeof(output_pair));
    array[0].operation = op;
    array[0].index = idx;
    pair_cnt++;
  }
  return array;
}

/* Add read lookup table record to table */
lut_record ** add_lut_record(lut_record **table, lut_record record) {

	lut_record *tmp_record = malloc(sizeof(lut_record));

	// Initialization
	tmp_record->idx = global_record_idx;
	memset(&tmp_record->addr[0], 0, sizeof(tmp_record->addr));	
	tmp_record->m_size = 0;
	tmp_record->thread_id = 0;
	memset(&tmp_record->var_name[0], 0, sizeof(tmp_record->var_name));
	memset(&tmp_record->func_name[0], 0, sizeof(tmp_record->func_name));
	tmp_record->scope = 0;
	tmp_record->var_type = 0;
	tmp_record->var_type2 = 0;
	tmp_record->heap_level = 0;
	tmp_record->line_num = 0;
	tmp_record->var_offset = 0;
	tmp_record->array_fl = 0;

	// Copy given values to tmp in order to store them in our look up table//
	tmp_record->idx = record.idx;
	strcpy(tmp_record->addr, record.addr);
	tmp_record->m_size = record.m_size;
	tmp_record->thread_id = record.thread_id;
	strcpy(tmp_record->var_name, record.var_name);
	strcpy(tmp_record->func_name, record.func_name);
	tmp_record->scope = record.scope;
	tmp_record->var_type = record.var_type;
	tmp_record->var_type2 = record.var_type2;
	tmp_record->heap_level = record.heap_level;
	tmp_record->line_num = record.line_num;
	tmp_record->var_offset = record.var_offset;
	tmp_record->array_fl = record.array_fl;

	global_record_idx++;

	table=realloc(table, global_record_idx*sizeof(lut_record));
	table[global_record_idx-1] = tmp_record;

	return table;
}

/* Free the allocated memory for the lookup table */
void free_lut(lut_record **table) {

  unsigned int ui;

  for(ui = 0; ui < global_record_idx; ui++) {
    free(table[ui]);  
  }
  free(table);
}

void write_to_lut_file(lut_record *record, FILE *fp){ 
  fwrite(record,sizeof(lut_record), 1, fp);
}

// Create a new record and allocate memory for it //
// Store every record on a file	//
lut_record ** create_lut_record(lut_record **table, record_line line, FILE *fp){
  lut_record *tmp_record;
	
	#ifdef INFO
		printf("Creating look up table record...\n");
	#endif
  tmp_record=malloc(sizeof(lut_record));
	//Added by Pavlos Aimoniotis 11/10/2019	//
//	if(line.var_scope == 'H')	{ // If a record belongs to a heap, every array position has different address. i.e. Starting address A, array[0] = A / array[1] = A + 4/ array[2] = A + 8 //
															// But for the purpose of memory access pattern, we want to analyze the main address of our allocation, which is the start point	//
//		tmp_record->idx = heap_idx;
//	}
// else{
		tmp_record->idx = global_record_idx;
//	}
  memset(&tmp_record->addr[0], 0, sizeof(tmp_record->addr));
  memset(&tmp_record->var_name[0], 0, sizeof(tmp_record->var_name));
  memset(&tmp_record->func_name[0], 0, sizeof(tmp_record->func_name));
	strcpy(tmp_record->addr, line.address);
	strcpy(tmp_record->var_name, line.var_name);
  strcpy(tmp_record->func_name, line.fn_nm);
  tmp_record->m_size = line.mem_size;
  tmp_record->thread_id = line.thread_id;
  tmp_record->scope = line.var_scope;
  tmp_record->var_type = line.var_scope2;
  tmp_record->var_type2 = line.var_type;
  tmp_record->heap_level = line.h_lvl;
  tmp_record->line_num = line.ln_n;
	tmp_record->var_offset = line.var_off;
  tmp_record->array_fl = line.array_fl;

  global_record_idx++;
  table = realloc(table,global_record_idx*sizeof(lut_record*));
  table[global_record_idx-1] = tmp_record; // Add it to table //

  write_to_lut_file(tmp_record,fp);	// Write the record on an output file	//
  return table;
}


/* Check lookup table for matching entries */
int check_lut_table(lut_record **table,record_line line){
  unsigned int duplicate_fl=0;
  zero_match_fl=0;
  unsigned int ui;

  for (ui = 0; ui < global_record_idx; ui++) {
   if (strcmp(line.var_name, table[ui]->var_name) == 0) {
      //if(line.var_off==table[ui]->var_offset) {
        if (line.var_scope == table[ui]->scope) { 
          //if((line.var_scope=='H') && (line.var_scope2==table[ui]->var_type)) {
						if (line.var_scope == 'H'){
            //if(!strcmp(line.address,table[ui]->addr)) {
              duplicate_fl = ui;
              if (ui == 0){
                zero_match_fl = 1;
              }
            //}
						}
						else {
							if ((line.var_scope == 'G') && (line.var_scope2 == table[ui]->var_type)) { 
								if (!strcmp(line.address, table[ui]->addr)) {
									if (line.thread_id == table[ui]->thread_id) {
										duplicate_fl = ui;
										if (ui == 0) {
											zero_match_fl = 1;
										}
									}
								}
							}
							if ((line.var_scope == 'S') && (line.var_scope2 == table[ui]->var_type)) {
								if (strcmp(line.fn_nm, table[ui]->func_name) == 0) {
									if (!strcmp(line.address, table[ui]->addr)) {
										duplicate_fl = ui;
										if (ui == 0) {
											zero_match_fl = 1;
										}
									}
								}
							}
						}
        }
      //}
    }
  }
  return duplicate_fl;
}
/* Print a record from the lookup table */
void print_lut_record(lut_record *ptr) {
  printf("Lookup table index: %d\n",ptr->idx);
  printf("Address: %s\n",ptr->addr);
  printf("Memory size: %d\n",ptr->m_size);
  printf("Thread id: %d\n",ptr->thread_id);
  printf("Variable name: %s\n",ptr->var_name);
  printf("Function name: %s\n",ptr->func_name);
  printf("Variable scope: %c\n",ptr->scope);
  if((ptr->scope=='G') || ptr->scope=='L') {
    printf("Variable type: %c\n",ptr->var_type);
  } else {
    printf("Heap level: %d\n",ptr->heap_level);
    printf("Line number in code: %d\n",ptr->line_num);
  }
  if(ptr->array_fl>0) {
    printf("Variable offset: %d\n",ptr->var_offset);
  }
  printf("\n");
}

/* Get the variable name of a dynamic variable from the filename and line number */
char * get_dyn_var_name(char * filename,int ln) {

  char name[1000];
  char *name2;
  char c=0;
  int cnt=2, filename_size;
  int cnt2=0;
  FILE *fptr, *temp_file;
	char command[1050];
	char *temp;

	#ifdef INFO
		printf("Trying to detect variable's name...\n");
	#endif
  name2 = malloc(100*sizeof(char));
  memset(&name2[0], 0, 100*sizeof(char)); 

	/*if (foundpath == 0){
		foundpath = 1;
		printf("Enter the path of executable source code (i.e. /my/path/in/computer/sourcecode): ");
		scanf("%s", &completepath);
	}*/

	/* Detect source code path	*/
	sprintf(command , "rm paths/filepath; find %s -name %s>paths/filepath",completepath, filename);
	system(command);
	temp_file = fopen("paths/filepath", "r");
	if(temp_file == NULL){
		printf("Error: %d (%s)\n", errno, strerror(errno));
		exit(1);         
	}
	fseek(temp_file, (size_t)0, SEEK_END);
	filename_size = ftell(temp_file);
	fseek(temp_file, 0, SEEK_SET);
	fgets(filename, filename_size, temp_file);
	if (fclose(temp_file) != 0 ){
		printf("Error: %d (%s)\n", errno, strerror(errno));
		exit(1);         
	}
	#ifdef INFO
		printf("Searching in file %s\n", filename);
	#endif
	if ((fptr = fopen(filename, "r")) == NULL) {
		printf("Error: %d (%s)\n", errno, strerror(errno));
		exit(1);         
	}
	
  while (fgets(name, sizeof name, fptr) != NULL) {
    if (cnt == ln) {
      break;
    } 
		else {
        cnt++;
    }
  }

  while(!isalpha(c)) {
    c=fgetc(fptr);
  }
  while(isalpha(c)) {
    name2[cnt2]=c;
    cnt2++;
    c=fgetc(fptr);
  }
  name2[cnt2]='\0';
	#ifdef INFO
		printf("Variable detected. Variable's name is %s\n", name2);
	#endif
  fclose(fptr);
  return name2;
}

/* Check the validity of the current line read from file  */
/* This function parses gleipnir's output									*/
record_line validate_line(char buffer[1024]){
  
  record_line temp;
  int buffer_idx, l, k, i, m, dot; // buffer_idx refers to the index on the buffer every and each time, while we are parsing it	//
  int fl=0, struct_fl=0;
  char current_char, tb[50], tnum[10];

	#ifdef INFO
		printf("Validating line...\n");
	#endif
  /* Initialization */
  memset(&temp.address[0], 0, sizeof(temp.address));
  memset(&temp.shoname[0], 0, sizeof(temp.shoname));
  memset(&temp.fn_nm[0], 0, sizeof(temp.fn_nm));
  memset(&temp.filename[0], 0, sizeof(temp.filename));
  memset(&temp.var_name[0], 0, sizeof(temp.var_name));
  memset(&temp.member[0], 0, sizeof(temp.member));
  temp.mem_size = 0;
  temp.thread_id = 0;
  temp.h_lvl = 0;
  temp.ln_n = 0;
  temp.var_off = 0;
  temp.var_scope = 0;
  temp.var_type = 0;
  temp.invalid_line = 0;
  temp.array_fl = 0;

	// An operation can be either Store or Load or Modify	//
  if ( buffer[0] == 'S' || buffer[0] == 'L' || buffer[0] == 'M' ) {
    temp.operation=buffer[0];// Memory operation
    for (buffer_idx = 0; buffer[buffer_idx+2] != ' '; buffer_idx++) {// Memory address
      temp.address[buffer_idx] = buffer[buffer_idx+2]; // buffer_idx+2, because the first byte is either S or L or M and the next is a space
    }

    temp.address[buffer_idx]='\0';
		while (!isspace(buffer[buffer_idx])){// Adjusting index
      buffer_idx++;
    }
    buffer_idx++;
    for (i = 0; i<20; i++) {
      tb[i]='\0';
    }

    k = 0;

    for(;!isspace(buffer[buffer_idx]);buffer_idx++) { // buffer_idx set before of ;
      tb[k] = buffer[buffer_idx];
      k++;
    }

    tb[k] = '\0';
    temp.mem_size=atoi(tb);// Memory size
    while (isspace(buffer[buffer_idx])) {// Adjusting index
      buffer_idx++;
    }

    m = 0;
    while (isdigit(buffer[buffer_idx])) {
      tnum[m] = buffer[buffer_idx];
      m++;
      buffer_idx++;
    }
    tnum[m] = '\0';

    temp.thread_id = atoi(tnum);// Thread Id
    while (isspace(buffer[buffer_idx])) {// Adjusting index
      buffer_idx++;
    }

    temp.var_scope = buffer[buffer_idx];// Variable scope
    if (buffer[buffer_idx] == 'H'){
      fl=1;// Heap space flag
    }
    if (buffer[buffer_idx] == 'M'){// Lines with M variable scope are to be ignored
      temp.invalid_line=1;    
    }

    while (!isspace(buffer[buffer_idx])) {// Adjusting index
      buffer_idx++;
    }
    while (isspace(buffer[buffer_idx])) {// Adjusting index
      buffer_idx++;
    }

    l=buffer_idx;
    for (; buffer[buffer_idx] != ':'; buffer_idx++) {    
       temp.shoname[buffer_idx-l] = buffer[buffer_idx]; // Shared object name
     }
    temp.shoname[l]='\0';

    if (strcmp(temp.shoname, "NONE")) {
      temp.invalid_line = 1;
    }

    buffer_idx = buffer_idx + 2;

    if((buffer[buffer_idx]==' ') || (buffer[buffer_idx]=='\0') || (buffer[buffer_idx]=='\n')) {
      temp.invalid_line=1;                            // If the line has no more data past this point it is invalid
    }

    while( (!isalnum(buffer[buffer_idx])) && temp.invalid_line==0) {// Adjusting index
      buffer_idx++;
    }
		
		// If the function does not have arguments then the next character will be space. If while checking the
		// fuction name it detects a starting parenthesis, then it will end with a closing parenthesis and not a space
		char endcharacter = ' ';
    for (l=buffer_idx; buffer[l] != endcharacter ; l++) {
			if(buffer[l] == '('){
				endcharacter = ')';
			}
      temp.fn_nm[l-buffer_idx] = buffer[l];// Function name
    }
	
		if (endcharacter == ' '){
			temp.fn_nm[l - buffer_idx] = '\0';
			buffer_idx = l;
		}
		else{
			temp.fn_nm[l - buffer_idx] = ')';
			temp.fn_nm[l - buffer_idx + 1] = '\0';
			buffer_idx = l + 1;
		};

    while(!isspace(buffer[buffer_idx])) {// Adjusting index
      buffer_idx++;
    }
    while(isspace(buffer[buffer_idx])) {// Adjusting index
      buffer_idx++;
    }

    if((buffer[buffer_idx]==' ') || (buffer[buffer_idx]=='\0') || (buffer[buffer_idx]=='\n')) {
      temp.invalid_line=1;// If the line has no more data past this point it is invalid
    }

		if(buffer[buffer_idx]=='['){
			while(buffer[buffer_idx]!=']'){
				buffer_idx++;
			}
			buffer_idx++;
			buffer_idx++;
		}


    if(fl == 1 && temp.invalid_line == 0) {// fl==1 when the variable scope is H 
      temp.var_scope2 = buffer[buffer_idx];// 2nd Variable scope 
      temp.var_type = '-';// Variable type
      while(!isspace(buffer[buffer_idx])) {// Adjusting index
      buffer_idx++;
      }
      while(isspace(buffer[buffer_idx])) {// Adjusting index
        buffer_idx++;
      }
      m = 0;
      dot = 0;

      while((buffer[buffer_idx]!='.') && temp.invalid_line==0) {
        temp.filename[m] = buffer[buffer_idx];// Filename
        m++;
        buffer_idx++;
        dot++;
      }

      temp.filename[m] = '\0';
      m--;
      k=0;
      while((isdigit(temp.filename[m])) && temp.invalid_line==0) {
        tnum[k] = temp.filename[m];
        k++;
        temp.filename[m] = '\0';  
        m--;
        dot--;
      }

      while((!isalnum(temp.filename[m])) && temp.invalid_line==0) {
        temp.filename[m] = '\0';
        m--;
      }
			if (strstr(temp.filename, "cpp")!=NULL){ //If filename contains cpp as name not for suffix, then good luck.
				dot = dot - 5;
			}
			else{
				dot = dot - 3;//Replacing the character '_' with '.' before 'c' for the proper filename 
			}
      tnum[k] = '\0';

      int pow_cnt = 0;
      int pow_prod;

      for (m = 0; m < k; m++){
        current_char = tnum[m];
        l = current_char - '0';
        pow_prod = 1; 
        for(i = 0; i < pow_cnt; i++){
          pow_prod = pow_prod * 10;
        }
        temp.ln_n = temp.ln_n+(l*pow_prod);// Line number
        pow_cnt++;
      }
      temp.filename[dot]='.';
      char *dyn_var_name;
      dyn_var_name=get_dyn_var_name(temp.filename,temp.ln_n);          // Dynamic variable names
      strcpy(temp.var_name,dyn_var_name);
      //strcpy(temp.var_name,temp.filename); //Deleted by Pavlos Aimoniotis 11/10/2019
      free(dyn_var_name);
      
      buffer_idx++;// Adjusting index
      l = buffer_idx;
      k = l;
      m = 0;
      while((isdigit(buffer[l])) && temp.invalid_line==0) {// Variable offset
        tnum[m] = buffer[l];
        m++;
        l++;
      }
      tnum[m] = '\0';
      temp.var_off = atoi(tnum);  
      for(i = 0; i < 10; i++) {
        tnum[i] = '\0';
      }
      m=0;

    }
    else {// fl==0 when the variable scope is G/S/M 
      //if(!isspace(buffer[buffer_idx])){// If the line has no more data past this point the line is invalid
			if (buffer[buffer_idx] != '\0' && buffer[buffer_idx] !='\n'){
        temp.var_scope2=buffer[buffer_idx];// 2nd Variable scope
        temp.var_type=buffer[buffer_idx+1];// Variable type
        buffer_idx=buffer_idx+3;
        strcpy(temp.filename,"-");

        for(i=0;i<50;i++) {
          tb[i]='\0';
          temp.member[i]='\0';
          tnum[i/5]='\0'; 
        }
        i=buffer_idx;
        m=0;
        if(buffer[buffer_idx]==' ') {
          buffer_idx++;
        }
        while( (buffer[i] != '\0') && (buffer[i] != '[') && (buffer[i] != '\n')) {
          tb[i-buffer_idx+m]=buffer[i];
          i++;
          buffer_idx++;
          m++;
        }
        strcpy(temp.var_name,tb);// Variable name
        i=0;
        
        if(buffer[buffer_idx]=='[') {
          temp.array_fl=1;
          buffer_idx++;
          while(buffer[buffer_idx]!=']') {
            tnum[i]=buffer[buffer_idx];
            buffer_idx++;
            i++;
          }
          tnum[i]='\0';
          buffer_idx++;// Adjusting index
          temp.var_off=atoi(tnum);// Variable offset
          for(i=0;i<10;i++){
            tnum[i]='\0';
          }
          if(buffer[buffer_idx]==' ') {
            buffer_idx++;
          }
        }

        if(buffer[buffer_idx]=='.') {
          struct_fl=1;
          for(i=0;i<20;i++){
            tb[i]='\0';
          }
          buffer_idx++;// Adjusting index
          i=0;
          while(buffer[buffer_idx] != '\0') {
            if((buffer[buffer_idx]!=']') && (buffer[buffer_idx]!='\0') && (buffer[buffer_idx]!='\n') && (buffer[buffer_idx]!=' ')) {
              tb[i]=buffer[buffer_idx];
              i++;
              buffer_idx++;
            }
            else {
              tb[i]=buffer[buffer_idx];
              break;
            }
          }
          strcpy(temp.member,tb);// Member name (if variable is a struct)
        }
      }
			else{temp.invalid_line = 1;}
    }
  }
  else {
    temp.invalid_line=1;// X accesses and irrelevant lines
  } 
  return temp;
}

/* Implementation of the line check , the lookup table creation and writing to output */
lut_record ** check_line(char buffer[1024],lut_record **tmp_table,FILE *lut){

  record_line tmp_line2;
  int duplicate_line=0;// 0: line doesn't exist on the lut  1: line exists on the lut

  tmp_line2 = validate_line(buffer); // Parse gleipnir's tool line	//

  if(tmp_line2.invalid_line == 0){
    if(global_record_idx == 0){ // first entry	//
      tmp_table = create_lut_record(tmp_table, tmp_line2, lut);
      pair_array = add_to_array(pair_array, tmp_line2.operation, duplicate_line);
    } 
		else {
      duplicate_line = check_lut_table(tmp_table, tmp_line2); // Check if an entry already exists	//
      if((duplicate_line == 0) && zero_match_fl == 0){ // If not then we should create a record	//
        tmp_table = create_lut_record(tmp_table, tmp_line2, lut);
        pair_array = add_to_array(pair_array, tmp_line2.operation, global_record_idx - 1);
      } 
			/*else if (duplicate_line !=0 && tmp_line2.var_scope == 'H'){
				tmp_table = create_lut_record(tmp_table, tmp_line2, lut);
				pair_array = add_to_array(pair_array, tmp_line2.operation, duplicate_line);
			}*/
			else {
				tmp_table = create_lut_record(tmp_table, tmp_line2, lut);
        pair_array = add_to_array(pair_array, tmp_line2.operation, duplicate_line);
      }
    }
  }
  return tmp_table;
}

/* Output file size presentation */
char* readable_fs(double size, char *buf) {
    int i = 0;
    const char* units[] = {"B", "kB", "MB", "GB", "TB", "PB"};
    while (size > 1024) {
        size /= 1024;
        i++;
    }
    sprintf(buf, "%.*f %s", i, size, units[i]);
    return buf;
}

/* For all allocations detects the malloc and the allocation point */
void detect_allocated_info(FILE *parsed_file)	{
	char buffer[1024], *pointer, variables[1000][10], lengths[1000][10], fullnamewithline[100];
	int variables_count=0, i, length_len;
	FILE *alloc_info;

	#ifdef INFO
		printf("Detecting allocated info...\n");
	#endif
  while (fgets(buffer, sizeof(buffer), parsed_file) != NULL) {
		if (buffer[0] != 'X'){ //If different does not need to check
			break;
		}
		pointer = strstr(buffer, "ALLOC "); //find allocated line (mALLOC, reALLOC, cALLOC)
		if (pointer != NULL){
			pointer = pointer + 6; // adjusting index on file
			strncpy(variables[variables_count], pointer ,9); //address always take 9 bytes (i.e. 0051f1030)
			pointer = pointer + 10; //adjusting index on file

			length_len = 0;
			while(pointer[length_len]!=' '){
				length_len++;
			}
			strncpy(lengths[variables_count], pointer, length_len);

			variables_count++;
		}
  }

	fseek(parsed_file, 0, SEEK_SET); //return in the beggining of the file
	
	alloc_info = fopen("alloc_info", "wb");
	for (i = 0; i < variables_count; i++){
		fprintf(alloc_info,"%s %s\n", variables[i], lengths[i]);
	}
	fclose(alloc_info);

}

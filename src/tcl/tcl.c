/* Memory Access Pattern Visualization */
// Author: Pavlos Aimoniotis
////////////////////////////////////////
// In this file all tcl commands are 
// initialized and specified. 
// Nothing special happens in this file.
// All functions contain the absolutely 
// necessary for the command to run.
// filtering_functions.c, memory_tracing,
// output_processing does all the job

#include "tcl.h"

int comSet_valgrind_path(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  const char syntax[] = "?<filename>";

  if (objc != 2)
		{
			printf("Wrong number of arguments! Command <set_valgrind_path> requires as argument <pathtovalgrindexe>.\n");
			printf("i.e.  set_valgrind_path /home/path/to/my/valgrind/bin/valgrind\n");
      Tcl_WrongNumArgs(interp, 1, (Tcl_Obj * const *)objv, syntax);
      return TCL_ERROR;
		}

	strcpy(valgrindpath, (char *)Tcl_GetString((Tcl_Obj *)objv[1]));
	printf(CYN"Valgrind path set to: %s\n"NRM, valgrindpath);
	return TCL_OK;
}
int comLoad_exe(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  const char syntax[] = "?<filename>";

  if (objc != 2)
		{
			printf("Wrong number of arguments! Command <load_exe> requires as argument <pathtoexe>.\n");
			printf("i.e.  load_exe /home/path/to/my/executalefilename\n");
      Tcl_WrongNumArgs(interp, 1, (Tcl_Obj * const *)objv, syntax);
      return TCL_ERROR;
		}

	strcpy(executablepath, (char *)Tcl_GetString((Tcl_Obj *)objv[1]));
	printf(CYN"Executable set to: %s\n"NRM, executablepath);
	return TCL_OK;
}

int comSet_src_path(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  const char syntax[] = "?<filename>";

  if (objc != 2)
		{
			printf("Wrong number of arguments! Command <set_src_path> requires as argument <path>.\n");
			printf("i.e. set_src_path <path>\n");
      Tcl_WrongNumArgs(interp, 1, (Tcl_Obj * const *)objv, syntax);
      return TCL_ERROR;
		}

	strcpy(completepath, (char *)Tcl_GetString((Tcl_Obj *)objv[1]));
	printf(CYN"Path set to: %s\n"NRM, completepath);
	return TCL_OK;
}

int comSet_arguments(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  const char syntax[] = "?<filename>";
	int i;
	if (objc == 1){
		printf("No arguments were set. To set arguments use set_arguments arg1 arg2..\n");
		return TCL_ERROR;
	}
	strcpy(arguments, (char *)Tcl_GetString((Tcl_Obj *)objv[1]));
	for (i=2; i < objc; i++)
		{
			strcat(arguments, " ");
			strcat(arguments, (char *)Tcl_GetString((Tcl_Obj *)objv[i]));
		}
	printf(CYN"Arguments set to: %s\n"NRM, arguments);
	return TCL_OK;
}

int comRun_gleipnir(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  const char syntax[] = "?<filename>";
	char command[5000];

  if (objc != 1)
		{
			printf("Wrong number of arguments! Command <run_geipnir> does not require arguments.\n");
			printf("Before running command, remember to set_valgrind_path, load_exe, set_src_path.\n");
      Tcl_WrongNumArgs(interp, 1, (Tcl_Obj * const *)objv, syntax);
      return TCL_ERROR;
		}
	if (strcmp(valgrindpath, " ")==0){
		printf("Path for valgrind has not been set, use <set_valgrind_path>!\n");
      return TCL_ERROR;
	}
	if (strcmp(executablepath, " ")==0){
		printf("Path for executable has not been set, use <load_exe>!\n");
      return TCL_ERROR;
	}
	printf("If gleipnir files exist, they will be removed.\n");
	system("rm gleipnir.*");
	sprintf(command, "%s --quiet --read-var-info=yes --read-debug=yes --tool=gleipnir %s %s", valgrindpath, executablepath, arguments);
	printf(CYN"Executing: %s\n"NRM, command);
	printf("This may take some time, please wait...\n");
	system(command);
	printf(CYN"Command %s COMPLETED\n"NRM, command);
	system("mv gleipnir.* gleipnir_out");
	printf(CYN"Gleipnir output stored in gleipnir_out.\n"NRM);
	return TCL_OK;
}

int comCachegrind(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  const char syntax[] = "?<filename>";
	char command[5000];

  if (objc != 1)
		{
			printf("Wrong number of arguments! Command <cachegrind> does not require arguments.\n");
			printf("Before running command, remember to set_valgrind_path, load_exe.\n");
      Tcl_WrongNumArgs(interp, 1, (Tcl_Obj * const *)objv, syntax);
      return TCL_ERROR;
		}
	if (strcmp(valgrindpath, " ")==0){
		printf("Path for valgrind has not been set, use <set_valgrind_path>!\n");
      return TCL_ERROR;
	}
	if (strcmp(executablepath, " ")==0){
		printf("Path for executable has not been set, use <load_exe>!\n");
      return TCL_ERROR;
	}
	printf("If cachgrind file exists, it will be removed.\n");
	system("rm grindanalysis/cachegrind");
	sprintf(command, "%s --tool=cachegrind --read-var-info=yes --cachegrind-out-file=grindanalysis/cachegrind %s %s", valgrindpath, executablepath, arguments);
	printf(CYN"Executing: %s\n"NRM, command);
	printf("This may take some time, please wait...\n");
	system(command);
	printf(CYN"Command %s COMPLETED\n"NRM, command);
	printf("Cachegrind output stored in grindanalysis/cachegrind.\n");
	return TCL_OK;
}

int comLackey(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  const char syntax[] = "?<filename>";
	char command[5000];

  if (objc != 1)
		{
			printf("Wrong number of arguments! Command <lackey> does not require arguments.\n");
			printf("Before running command, remember to set_valgrind_path, load_exe.\n");
      Tcl_WrongNumArgs(interp, 1, (Tcl_Obj * const *)objv, syntax);
      return TCL_ERROR;
		}
	if (strcmp(valgrindpath, " ")==0){
		printf("Path for valgrind has not been set, use <set_valgrind_path>!\n");
      return TCL_ERROR;
	}
	if (strcmp(executablepath, " ")==0){
		printf("Path for executable has not been set, use <load_exe>!\n");
      return TCL_ERROR;
	}
	printf("If lackey file exists, it will be removed.\n");
	system("rm grindanalysis/lackey");
	sprintf(command, "%s --tool=lackey --read-var-info=yes --detailed-counts=yes --trace-mem=yes --log-file=grindanalysis/lackey %s %s ", valgrindpath, executablepath, arguments);
	printf(CYN"Executing: %s\n"NRM, command);
	printf("This may take some time, please wait...\n");
	system(command);
	printf(CYN"Command %s COMPLETED\n"NRM, command);
	printf("Lackey output stored in grindanalysis/lackey.\n");
	return TCL_OK;
}

int comDhat(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  const char syntax[] = "?<filename>";
	char command[5000];

  if (objc != 1)
		{
			printf("Wrong number of arguments! Command <lackey> does not require arguments.\n");
			printf("Before running command, remember to set_valgrind_path, load_exe.\n");
      Tcl_WrongNumArgs(interp, 1, (Tcl_Obj * const *)objv, syntax);
      return TCL_ERROR;
		}
	if (strcmp(valgrindpath, " ")==0){
		printf("Path for valgrind has not been set, use <set_valgrind_path>!\n");
      return TCL_ERROR;
	}
	if (strcmp(executablepath, " ")==0){
		printf("Path for executable has not been set, use <load_exe>!\n");
      return TCL_ERROR;
	}
	printf("If dhat file exists, it will be removed.\n");
	system("rm grindanalysis/dhat");
	sprintf(command, "%s --tool=dhat --read-var-info=yes --log-file=grindanalysis/dhat %s %s ", valgrindpath, executablepath, arguments);
	printf(CYN"Executing: %s\n"NRM, command);
	printf("This may take some time, please wait...\n");
	system(command);
	printf(CYN"Command %s COMPLETED\n"NRM, command);
	printf("Dhat output stored in grindanalysis/dhat.\n");
	return TCL_OK;
}

int comMassif(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  const char syntax[] = "?<filename>";
	char command[5000];

  if (objc != 1)
		{
			printf("Wrong number of arguments! Command <lackey> does not require arguments.\n");
			printf("Before running command, remember to set_valgrind_path, load_exe.\n");
      Tcl_WrongNumArgs(interp, 1, (Tcl_Obj * const *)objv, syntax);
      return TCL_ERROR;
		}
	if (strcmp(valgrindpath, " ")==0){
		printf("Path for valgrind has not been set, use <set_valgrind_path>!\n");
      return TCL_ERROR;
	}
	if (strcmp(executablepath, " ")==0){
		printf("Path for executable has not been set, use <load_exe>!\n");
      return TCL_ERROR;
	}
	printf("If massif file exists, it will be removed.\n");
	system("rm grindanalysis/massif");
	sprintf(command, "%s --tool=massif --massif-out-file=grindanalysis/massif %s %s ", valgrindpath, executablepath, arguments);
	printf(CYN"Executing: %s\n"NRM, command);
	printf("This may take some time, please wait...\n");
	system(command);
	printf(CYN"Command %s COMPLETED\n"NRM, command);
	printf("Massif output stored in grindanalysis/massif.\n");
	return TCL_OK;
}

int comHelgrind(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  const char syntax[] = "?<filename>";
	char command[5000];

  if (objc != 1)
		{
			printf("Wrong number of arguments! Command <helgrind> does not require arguments.\n");
			printf("Before running command, remember to set_valgrind_path, load_exe.\n");
      Tcl_WrongNumArgs(interp, 1, (Tcl_Obj * const *)objv, syntax);
      return TCL_ERROR;
		}
	if (strcmp(valgrindpath, " ")==0){
		printf("Path for valgrind has not been set, use <set_valgrind_path>!\n");
      return TCL_ERROR;
	}
	if (strcmp(executablepath, " ")==0){
		printf("Path for executable has not been set, use <load_exe>!\n");
      return TCL_ERROR;
	}
	printf("If helgrind file exists, it will be removed.\n");
	system("rm grindanalysis/helgrind");
	sprintf(command, "%s --tool=helgrind --read-var-info=yes --log-file=grindanalysis/helgrind %s %s ", valgrindpath, executablepath, arguments);
	printf(CYN"Executing: %s\n"NRM, command);
	printf("This may take some time, please wait...\n");
	system(command);
	printf(CYN"Command %s COMPLETED\n"NRM, command);
	printf("Helgrind output stored in grindanalysis/helgrind.\n");
	return TCL_OK;
}
int comSet_mem_alloc(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  const char syntax[] = "?<filename>";
	int i;
	if (objc != 2){
		printf("Wrong number of arguments! Command <set_mem_alloc> requires argument either 'S' for stack  or 'H'.\n");
		return TCL_ERROR;
	}
	if (strcmp((char *)Tcl_GetString((Tcl_Obj *)objv[1]), "H")!=0 && strcmp((char *)Tcl_GetString((Tcl_Obj *)objv[1]),"S")!=0)
		{
			printf("<set_mem_alloc S> for setting memory allocation to Stack or <set_mem_alloc H> for setting memory allocation to Heap. \n");
			return TCL_ERROR;
		}

	strcpy(stackorheap, (char *)Tcl_GetString((Tcl_Obj *)objv[1]));
	if(strcmp(stackorheap,"S")==0)
		{
			printf(CYN"Memory allocation to be analysed set to stack.\n"NRM);
		}
	else
		{
			printf(CYN"Memory allocation to be analysed set to heap.\n"NRM);

		}
	return TCL_OK;
}

int comMemory_trace_analysis(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  const char syntax[] = "?<filename>";
	char command[2000];
	if (objc != 1){
		printf("Wrong number of arguments! Command <memory_tracing_analysis> does not require arguments.\n");
		printf("Make sure you first <run_gleipnir>.\n");
		return TCL_ERROR;
	}
	sprintf(command, "src/analysisprocess/memtrace_analysis gleipnir_out %s", completepath);
	printf(CYN"Executing: %s\n"NRM, command);
	printf(CYN"This may take some time. Please wait...\n"NRM);
	system(command);
	printf(CYN"Command %s COMPLETED!\n"NRM, command);
	system("cat additional_info.txt");	

	return TCL_OK;
}

int comFiltering_process(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  const char syntax[] = "?<filename>";
	char command[2000];
	if (objc != 1){
		printf("Wrong number of arguments! Command <filtering_process> does not require arguments.\n");
		printf("Make sure you have set memory allocation with command <set_mem_alloc 'Allocdone'>.\n");
		return TCL_ERROR;
	}
	sprintf(command, "src/analysisprocess/output_processing %s", stackorheap);
	printf(CYN"Executing: %s\n"NRM, command);
	printf(CYN"This may take some time. Please wait...\n"NRM);
	system("ulimit -s unlimited");
	system(command);
	printf(CYN"Command %s COMPLETED!\n"NRM, command);
	printf(CYN"See memory information by typing tcl command <memory_stats>\n"NRM);

	return TCL_OK;
}

int comMemory_stats(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  const char syntax[] = "?<filename>";
	char command[2000];
	if (objc != 1){
		printf("Wrong number of arguments! Command <memory_stats> does not require arguments.\n");
		printf("Make sure you have executed command <filtering_process>.\n");
		return TCL_ERROR;
	}
	system("cat memory_stats");
	return TCL_OK;
}

int comComplete_memory_access_pattern(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  const char syntax[] = "?<filename>";
	if (objc == 1){
		system("python src/visualizationscripts/memory_access_pattern.py");
	}
	else{
		if (strcmp((char *)Tcl_GetString((Tcl_Obj *)objv[1]), "save") == 0)
			{
				system("python src/visualizationscripts/memory_access_pattern.py > visinfo/memory_access_pattern_info");
				printf("Memory access pattern visualization information has been stored on visinfo/memory_access_pattern_info.\n");
			}
		else
			{
				printf("Wrong argument. <complete_memory_access_pattern> can be used with or without args.\n");
				printf("No arguments, will make info be shown in screen.\n");
				printf("Use 'save', to save data on 'memory_access_pattern_info'.\n");
				return TCL_ERROR;
			}
	}
	return TCL_OK;
}

int com2darray_memory_access_pattern(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  const char syntax[] = "?<filename>";
	char command[1000], savecommand[1100];
	char arrname[1000];
	char colstotal[1000];
	char byteoftype[5];
	if (objc != 4 && objc != 5){
			printf("Wrong arguments. <2darray_memory_access_pattern> requires three arguments.\n");
			printf("arg1. <array name>\narg2. total columns of array\narg3. bytes of type (i.e. 4 for int etc)\n");
			printf("i.e. 2darray_memory_access_pattern arrayofintegers 32 4\n");
			printf("To save the info on visinfo folder use as last argument 'save'.\n");
			printf("i.e. 2darray_memory_access_pattern arrayofintegers 32 4 save\n");
			return TCL_ERROR;
	}

	if (strcmp(stackorheap, "S") == 0){
		printf("You can not visualize arrays on Stack allocation at the moment, sorry.\n");
		return TCL_ERROR;
	}
	
	strcpy(arrname, (char *)Tcl_GetString((Tcl_Obj *)objv[1]));
	strcpy(colstotal, (char *)Tcl_GetString((Tcl_Obj *)objv[2]));
	strcpy(byteoftype, (char *)Tcl_GetString((Tcl_Obj *)objv[3]));

	sprintf(command, "python src/visualizationscripts/2d_memory_access_pattern.py %s %s %s", arrname, colstotal, byteoftype);
	sprintf(savecommand, "%s > visinfo/2darray_memory_access_pattern_info", command);

	if(objc == 4){
		system(command);
	}
	else if (objc == 5){
		if (strcmp((char *)Tcl_GetString((Tcl_Obj *)objv[4]), "save") == 0){
			system(savecommand);
			printf("2d Memory access pattern visualization information has been stored on visinfo/2d_memory_access_pattern_info.\n");
		}
		else if (strcmp((char *)Tcl_GetString((Tcl_Obj *)objv[4]), "save") != 0){
			printf("Wrong arguments.\n");
			printf("arg1. <array name>\narg2. total columns of array\narg3. bytes of type (i.e. 4 for int etc)\n");
			printf("i.e. 2darray_memory_access_pattern arrayofintegers 32 4\n");
			printf("To save the information use and 'save' as last argument.\n");
			printf("i.e. 2darray_memory_access_pattern arrayofintegers 32 4 save\n");
			return TCL_ERROR;
		}
	}
	return TCL_OK;
}

int com3darray_memory_access_pattern(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  const char syntax[] = "?<filename>";
	char command[1000], savecommand[1100];
	char arrname[1000];
	char colstotal[1000];
	char depth[1000];
	char byteoftype[5];
	if (objc != 5 && objc != 6){
			printf("Wrong arguments. <3darray_memory_access_pattern> requires four arguments.\n");
			printf("arg1. <array name>\narg2. total columns of array(j)\narg3. depth of array (k)\narg4. bytes of type (i.e. 4 for int etc)\n");
			printf("i.e. 2darray_memory_access_pattern arrayofintegers 32 32 4 \n");
			printf("To save the info on visinfo folder use as last argument 'save'.\n");
			printf("i.e. 2darray_memory_access_pattern arrayofintegers 32 32 4 save\n");
			return TCL_ERROR;
	}

	if (strcmp(stackorheap, "S") == 0){
		printf("You can not visualize arrays on Stack allocation at the moment, sorry.\n");
		return TCL_ERROR;
	}
	strcpy(arrname, (char *)Tcl_GetString((Tcl_Obj *)objv[1]));
	strcpy(colstotal, (char *)Tcl_GetString((Tcl_Obj *)objv[2]));
	strcpy(depth, (char *)Tcl_GetString((Tcl_Obj *)objv[3]));
	strcpy(byteoftype, (char *)Tcl_GetString((Tcl_Obj *)objv[4]));

	sprintf(command, "python src/visualizationscripts/3d_memory_access_pattern.py %s %s %s %s", arrname, colstotal, depth, byteoftype);
	sprintf(savecommand, "%s > visinfo/3darray_memory_access_pattern_info", command);

	if(objc == 5){
		system(command);
	}
	else if (objc == 6){
		if (strcmp((char *)Tcl_GetString((Tcl_Obj *)objv[5]), "save") == 0){
			system(savecommand);
			printf("3d Memory access pattern visualization information has been stored on visinfo/3d_memory_access_pattern_info.\n");
		}
		else if (strcmp((char *)Tcl_GetString((Tcl_Obj *)objv[5]), "save") != 0){
			printf("Wrong arguments.\n");
			printf("arg1. <array name>\narg2. total columns of array(j)\narg3. depth of array (k)\narg4. bytes of type (i.e. 4 for int etc)\n");
			printf("i.e. 2darray_memory_access_pattern arrayofintegers 32 32 4 \n");
			printf("To save the info on visinfo folder use as last argument 'save'.\n");
			printf("i.e. 2darray_memory_access_pattern arrayofintegers 32 32 4 save\n");
			return TCL_ERROR;
		}
	}
	return TCL_OK;
}
int comClear_analysis(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  const char syntax[] = "?<filename>";
	char command[2000];
	if (objc != 1){
		printf("Wrong number of arguments! Command <clear analysis> does not require arguments.\n");
		return TCL_ERROR;
	}

	system("rm cache_out.txt -f;rm memory_stats -f;rm order_data.txt -f;rm additional_info.txt -f;rm gleipnir_out -f;rm tmp_path.txt -f;rm lut_file -f;rm access_list -f;rm alloc_info -f;");
	printf("Clear analysis completed.\n");
	return TCL_OK;
}
int comHelp(ClientData clientData, Tcl_Interp *interp,int objc, Tcl_Obj *CONST objv[])
{
  char *temp;
  const char syntax[] = "?<filename>";

  if (objc != 1){
      Tcl_WrongNumArgs(interp, 1, (Tcl_Obj * const *)objv, syntax);
      return TCL_ERROR;
  }

  printf("\n\n// ******************** //\n");
  printf("load_exe <path>\tset path to executable file\n");
  printf("set_src_path <path>\tset path to source file directory\n");
  printf("set_valgrind_path <path>\tset path to valgrind executable (usually /home/yourpath/valgrind/bin/valgrind)\n");
  printf("set_arguments <args>\tset arguments for executable if needed\n");
  printf("set_mem_alloc <alloc>\tS for stack, H for heap, sets which memory allocations to analyse\n");
  printf("run_gleipnir\truns memory tracking\n");
  printf("memory_trace_analysis\tanalysis memory tracking and produces files with information\n");
  printf("filtering_process\tfilters the output files of memory trace\n");
  printf("memory_stats\tneeds filtering_process to have been completed. Shows memory usage stats\n");
  printf("complete_memory_access_pattern <save(optional)>\tmemory access pattern for the whole execution of the program\n");
  printf("2darray_memory_access_pattern <arrayname> <cols> <typeofbytes> <save(optional>\tvisualizes map of a given 2d array\n");
  printf("3darray_memory_access_pattern <arrayname> <cols> <depth> <typeofbytes> <save(optional>\t visualizes map of a given 3d array\n");
  printf("clear_analysis\tremoves all files that were created\n");
  printf("run_gleipnir\truns memory tracking\n");

  printf("\n\n\n");

  printf("// ****Valgrind Tools***//\n");
	printf("1. load_exe and set_valgrind_path\n");
	printf("lackey\truns executable through lackey valgrind plugin\n");
	printf("helgrind\truns executable through helgrind valgrind plugin\n");
	printf("cachegrind\truns executable through cachegrind valgrind plugin\n");
	printf("massif\truns executable through massif valgrind plugin\n");
	printf("dhat\truns executable through dhat valgrind plugin\n");

  printf("\n\n\n");

	printf("use upper-lower arrows for history\n");
	printf("use tab for auto-completion\n");
	printf("use double-tab to see all commands\n");

  printf("\n\n\n");

  printf("// ******************* //\n");
  printf("###   flow example   ###\n");
  printf("load_exe\n");
  printf("set_src_path\n");
  printf("set_valgrind_path\n");
  printf("set_arguments\n");
  printf("set_mem_alloc\n");
  printf("run_gleipnir\n");
  printf("memory_trace_analysis\n");
  printf("filtering_process\n");
  printf("complete_memory_access_pattern\n");

	return TCL_OK;
}

int tcladdition(Tcl_Interp * interp)
{
	Tcl_CreateObjCommand(interp,"set_valgrind_path",comSet_valgrind_path,NULL,NULL);
	Tcl_CreateObjCommand(interp,"load_exe",comLoad_exe,NULL,NULL);
	Tcl_CreateObjCommand(interp,"set_src_path",comSet_src_path,NULL,NULL);
	Tcl_CreateObjCommand(interp,"set_arguments",comSet_arguments,NULL,NULL);
	Tcl_CreateObjCommand(interp,"run_gleipnir",comRun_gleipnir,NULL,NULL);
	Tcl_CreateObjCommand(interp,"set_mem_alloc",comSet_mem_alloc,NULL,NULL);
	Tcl_CreateObjCommand(interp,"memory_trace_analysis",comMemory_trace_analysis,NULL,NULL);
	Tcl_CreateObjCommand(interp,"filtering_process",comFiltering_process,NULL,NULL);
	Tcl_CreateObjCommand(interp,"clear_analysis",comClear_analysis,NULL,NULL);
	Tcl_CreateObjCommand(interp,"complete_memory_access_pattern",comComplete_memory_access_pattern,NULL,NULL);
	Tcl_CreateObjCommand(interp,"2darray_memory_access_pattern",com2darray_memory_access_pattern,NULL,NULL);
	Tcl_CreateObjCommand(interp,"3darray_memory_access_pattern",com3darray_memory_access_pattern,NULL,NULL);
	Tcl_CreateObjCommand(interp,"cachegrind",comCachegrind,NULL,NULL);
	Tcl_CreateObjCommand(interp,"lackey",comLackey,NULL,NULL);
	Tcl_CreateObjCommand(interp,"helgrind",comHelgrind,NULL,NULL);
	Tcl_CreateObjCommand(interp,"dhat",comDhat,NULL,NULL);
	Tcl_CreateObjCommand(interp,"massif",comMassif,NULL,NULL);
	Tcl_CreateObjCommand(interp,"memory_stats",comMemory_stats,NULL,NULL);
	Tcl_CreateObjCommand(interp,"help",comHelp,NULL,NULL);

return 0;
}

/* Memory Access Pattern Visualization */
// Author: Pavlos Aimoniotis
////////////////////////////////////////
// Main file of tool

#include <stdio.h>
#include <string.h>
#include "libs/structs.h"
#include "libs/functions.h"
#include <stdlib.h>
#include <tcl.h>
#include "tab/tabcompletion.h"
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h>


static void catch_kb_interrupts(int signal);

Tcl_Interp * interp;

int main (int argc , char * argv[])
{
	int i, j, tclw;

	char cwd[LINE_MAX]; // current working directory //
	char prompt[LINE_MAX]; // command prompt //


	HIST_ENTRY **the_history_list; // readline commands history list - NULL terminated //
	int expansionresult;
	char *text;
	char *textexpansion;
	char command[LINE_MAX];

	strcpy(valgrindpath," ");
	strcpy(executablepath," ");
	strcpy(arguments," ");
	strcpy(stackorheap,"S");
	
	printf(CYN"\nMemory Access Pattern Visualization is currently running!\nType <help> for running flow and command explanation.\n"NRM);
	rl_completion_entry_function = NULL; // use rl_filename_completion_function(), the default filename completer //
	rl_attempted_completion_function = command_completion;

	using_history(); // initialise history functions //

	// must execute this first for Tcl's reasons //
	Tcl_FindExecutable(argv[0]);

	interp = Tcl_CreateInterp();

	if ( interp == NULL )
		{
			fprintf(stderr, "ERROR: Could not create TCL interpreter!\n") ;
			return 1 ;
		}

	tclw = tcladdition(interp);

	while(1)
		{
			getcwd(cwd, LINE_MAX);
			sprintf(prompt, "[%s]%> ", cwd);

			text = readline(prompt);

			if (command != NULL)
				{
				expansionresult = history_expand(text, &textexpansion);

				if ((expansionresult == 0) || // no expansion //
				(expansionresult == 2)) // do not execute //
					{
					if (strcmp(textexpansion, "") != 0) // ignore empty strings for history //
					add_history(textexpansion);
					      
					strcpy(command, text); // store command //	      
					}

					free(textexpansion);
					free(text);
				}

			#ifdef DEBUGMODE
			{
			  printf("DEBUG: command = %s\n", command);
			}
			#endif

			if (strcmp(command, "history") == 0) // history command //
				{
					the_history_list = history_list(); // get history list //
					  
					if (the_history_list != NULL)
						{
							i = 0;
							while (*(the_history_list + i) != NULL) // history list - NULL terminated //
								{
									printf("%lu: %s\n", (i + history_base), (*(the_history_list + i))->line);
									i++;
								}
						}
				}
			else if ((strcmp(command ,"exit") == 0) || (strcmp(command ,"quit") == 0))
				{
				  Tcl_DeleteInterp(interp);
					//Add files to remove towards exit here
					system("rm cache_out.txt -f;rm scatterplot_data.txt -f;rm lut_stats_data.txt -f;rm order_data.txt -f;rm additional_info.txt -f;rm stats.txt -f; rm gleipnir_out -f;rm tmp_path.txt -f;rm lut_file -f;rm access_list -f;rm alloc_info -f;");
					break;
				}
			else
				{	  
				  Tcl_Eval(interp, command);
				}
		}

	printf("Program terminating. Goodbye!\n");
	return 0;
	}

/* handles the registered signals*/
static void catch_kb_interrupts(int signal)
{
//TODO :: catch each signal in each case and act respectively
//FYI: SIGINT is 2, SIGQUIT is 3, SIGTERM is 15

	printf("interrupt handler fired\n");

	Tcl_DeleteInterp(interp);

	exit(-1);

}


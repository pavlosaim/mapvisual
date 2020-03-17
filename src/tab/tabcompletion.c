/* Memory Access Pattern Visualization */
// Author: Pavlos Aimoniotis
////////////////////////////////////////
// Tab initialization on tcl shell.

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "tabcompletion.h"

char *command_names[] = {
		"set_path",
		"set_source_path",
		"set_valgrind_path",
		"run_gleipnir",
		"set_arguments",
		"set_mem_alloc",
		"memory_trace_analysis",
		"filtering_process",
		"clear_analysis",
		"complete_memory_access_pattern",
		"2darray_memory_access_pattern",
		"3darray_memory_access_pattern",
		"help",
    NULL
};

char **command_completion(const char *text, int start, int end)
{
  if (start == 0)
    {
      return rl_completion_matches(text, command_generator);
    }
  else
    {
      return NULL; // perform Readline's default completer //
    }
}

char *command_generator(const char *text, int state)
{
    static int list_index, len;
    char *name;

    if (!state)
      {
        list_index = 0;
        len = strlen(text);
      }

    while ((name = command_names[list_index++]))
      {
        if (strncmp(name, text, len) == 0) {
          return strdup(name);
        }
      }

    return NULL;
}


#ifndef TABCOMPLETION_H
#define TABCOMPLETION_H

char **command_completion(const char *text, int start, int end);
char *command_generator(const char *text, int state);


#endif

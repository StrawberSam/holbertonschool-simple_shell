#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

extern char **environ;
void cleaner(char *line);
void cleanup(char *line, char **args);
void handle_env(void);
void execute_command(char **args, char **av);
char **split_line(char *line);
char *_which(const char *cmd);
int main(int ac, char **av);
int handle_exit(char **args);

#endif /* MAIN_H */

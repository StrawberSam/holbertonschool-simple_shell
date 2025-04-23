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
char *get_line(void);
char **split_line(char *line);
char *find_command_in_path(const char *cmd);
int main(int ac, char **av);
void handle_no_args_command(char *line);
int handle_exit(char **args);
void handle_env();
void execute_command(char **args, pid_t pid, char **environ, char *line);
void handle_simple_command(char *line);

#endif /* MAIN_H */

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
void handle_no_args_command(char *line);
void handle_env();
void execute_command(char **args, pid_t pid, char **environ, char *line);
void handle_simple_command(char *line);
void simple_command(char *line);
void execute_command_interactive(char **args, char **av);
void handle_non_interactive(char **av);
char *get_line(void);
char **split_line(char *line);
char *find_command_in_path(const char *cmd);
int main(int ac, char **av);
int handle_exit(char **args);
ssize_t read_line(char **line, size_t *len);

#endif /* MAIN_H */

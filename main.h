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

char *get_line(void);
int main(void);
char **split_line(char *line);
char *find_command_in_path(const char *cmd);


#endif /* MAIN_H */

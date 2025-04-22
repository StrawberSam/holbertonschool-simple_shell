#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int get_line(void);
int main(void);
char **split_line(char *line);

#endif /* MAIN_H */

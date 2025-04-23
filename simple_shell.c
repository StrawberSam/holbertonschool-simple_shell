#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern char **environ;

/**
 * main - Shell version 0 : exécute des commandes simples
 *
 * Return: Always 0.
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t n_read;
	char *cmd[2];

	while (1)
	{
		printf("$ ");

		n_read = getline(&line, &len, stdin); /* Lecture de la commande */
		if (n_read == -1) /* Ctrl + D */
		{
			free(line);
			exit(0);
		}

		if (line[n_read - 1] == '\n')
			line[n_read - 1] = '\0'; /* Suppression du \n */

		cmd[0] = line;
		cmd[1] = NULL;

		if (execve(line, cmd, environ) == -1)
			perror("simple_shell");
	}

	free(line);
	return (0);
}

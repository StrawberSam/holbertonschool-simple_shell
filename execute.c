#include "main.h"

/**
 * execute_command - Exécute une commande en utilisant fork et execve
 * @args: Tableau d'arguments de la commande
 * @av: Tableau contenant argv[0] (nom du programme)
 */
void execute_command(char **args, char **av)
{
	pid_t pid;
	char *full_path;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) /* Processus enfant */
	{
		if (args[0][0] == '/' || args[0][0] == '.')
		{
			if (access(args[0], X_OK) == 0)
				execve(args[0], args, environ);

			perror(args[0]);
			exit(127);
		}

		full_path = find_command_in_path(args[0]);
		if (full_path)
		{
			execve(full_path, args, environ);
			perror(args[0]);
			free(full_path);
			exit(127);
		}
		else
		{
			fprintf(stderr, "%s: 1: %s: not found\n", av[0], args[0]);
			exit(127);
		}
	}
	else
		wait(NULL); /* Processus parent : attendre la fin */
}

/**
 * execute_simple_command - Exécute une commande sans arguments
 * @line: La commande à exécuter
 */

void execute_simple_command(char *line)
{
	pid_t pid;
	char *args[2];
 
	args[0] = line;
	args[1] = NULL;
 
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
 
	if (pid == 0) /* Processus enfant */
	{
		if (access(line, X_OK) == 0)
			execve(line, args, environ);
 
		perror(line);
		cleanup(line, args);
		exit(127);
	}
	else
		wait(NULL); /* Processus parent */
}

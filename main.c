#include "main.h"

/**
 * handle_exit - Vérifie si la commande est "exit"
 * @args: Tableau d'arguments
 *
 * Return: 1 si c'est "exit", 0 sinon
 */
int handle_exit(char **args)
{
	if (args[0] != NULL && strcmp(args[0], "exit") == 0)
		return (1);
	return (0);
}

/**
 * handle_env - Affiche les variables d'environnement
 *
 * @args: Un tableau d'arguments passés à la commande
 * @line: La ligne d'entrée à libérer
 *
 * Return: Aucun, car la fonction retourne void.
 */
void handle_env()
{
	int i = 0;

	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
}

void execute_command(char **args, pid_t pid, char **environ, char *line)
{
	pid = fork();
	if (pid == -1)  /* Si le fork échoue */
	{
		perror("fork");
		free(line);
		free(args);
		exit(1);
	}

	if (pid == 0)  /* Dans le processus enfant */
	{
		char *full_path = NULL;

		if (args[0] != NULL && (args[0][0] == '.' || args[0][0] == '/'))
		{
			if (access(args[0], X_OK) == 0)/*Vérifie si la cmd est exécutable*/
				execve(args[0], args, environ);
			else
			{
				perror("./shell");
				free(line);
				free(args);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			full_path = find_command_in_path(args[0]);
			if (full_path != NULL)
			{
				execve(full_path, args, environ);
				perror(args[0]);
				free(full_path);
			}
			else
			{
				fprintf(stderr, "%s: 1: %s: not found\n", args[0], args[0]);
			}
		}

		free(line);
		free(args);
		exit(127);
	}
	else  /* Dans le processus parent */
	{
		wait(NULL);  /* Attendre la fin du processus enfant */
	}
}

/**
 * handle_simple_command - Gère les commandes simples sans espaces.
 * @line: Ligne de commande entrée par l'utilisateur.
 * @pid: ID du processus enfant.
 */
void handle_simple_command(char *line)
{
		pid_t pid = fork();
		extern char **environ;
		char *args[2];
		args[0] = line;
		args[1] = NULL;

		if (pid == 0)
		{
			if (access(line, X_OK) == 0)
				execve(line, args, environ);

			else
			{
				fprintf(stderr, "%s: No such file or directory\n", line);
				exit(127);
			}
		}
		else
			wait(NULL);
}

/**
 * main - Point d'entrée du programme shell.
 * @ac: Nombre d'arguments.
 * @av: Tableau des arguments passés au programme.
 *
 * Return: Toujours 0 (Succès).
 */
int main(int ac, char **av)
{
	char *line = NULL;  /* Stocke la ligne d'entrée de l'utilisateur */
	char **args;        /* Stocke les tokens extraits de la ligne d'entrée */
	pid_t pid = 0;          /* Stocke l'ID du processus enfant */
	(void)ac; /* Paramètre inutilisé */
	(void)av;

	while (1)
	{
		/* Si le programme est en mode interactif, afficher l'invite */
		if (isatty(STDIN_FILENO))
		{
			printf("$ ");
			fflush(stdout);
		}
		else
			handle_non_interactive(av);

		line = get_line();  /* Récupérer la ligne d'entrée */
		if (line == NULL)
			continue;

		/* Vérifier si c'est une commande simple sans espace */
		args = split_line(line);
		if (!args)
		{
			free(line);
			continue;
		}

		args = split_line(line);  /* Découper la ligne en tokens */
		if (!args)
		{
			free(line);
			continue;
		}

		/* Gérer les commandes comme exit et env */
		if (handle_exit(args))
		{
			free(line);
			free(args);
			exit(0);
		}
		else if (strcmp(args[0], "env") == 0)
		{
			handle_env(args, line);
		}
		else
		{
			execute_command(args, pid, environ, line);  /* Exécuter les autres cmd */
		}

	}

	return (0);
}

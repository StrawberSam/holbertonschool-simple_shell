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
	size_t len = 0;
	char **args;        /* Stocke les tokens extraits de la ligne d'entrée */
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

		getline(&line, &len, stdin);  /* Récupérer la ligne d'entrée */
		cleaner(line);
		if (line == NULL)
			continue;

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
			execute_command(args, av); /* Exécuter les autres cmd */
		}

	}

	return (0);
}

#include "main.h"

/**
 * handle_exit - Vérifie si la commande est "exit"
 * @args: Tableau d'arguments
 * Return: 1 si c'est "exit", 0 sinon
 */
int handle_exit(char **args)
{
	/*vérifie si le 1e arg est vide et si = "exit"*/
	if (args[0] != NULL && strcmp(args[0], "exit") == 0)
		return (1); /*si oui, renvoie 1*/
	return (0); /*sinon, renvoie 0*/
}
/**
 * handle_env - Affiche les variables d'environnement
 * Return: Aucun, car la fonction retourne void.
 */
void handle_env(void)
{
	int i = 0; /*variable pour la boucle*/

	while (environ[i]) /*tant qu'il y a des variables d'env*/
	{
		printf("%s\n", environ[i]); /*affiche la variable d'env*/
		i++; /*on avance dans le PATH*/
	}
}

/**
 * main - Point d'entrée du programme shell.
 * @ac: Nombre d'arguments.
 * @av: Tableau des arguments passés au programme.
 * Return: Toujours 0 (Succès).
 */
int main(int ac, char **av)
{
	ssize_t read; /*variable pour la lecture*/
	char *line = NULL; /*variable pour la ligne de commande*/
	size_t len = 0; /*longueur de la ligne*/
	char **args; /*tableau d'arguments*/
	int i; /*variable pour la boucle*/
	(void)ac; /*non utilisé*/

	while (1) /*boucle infinie pr récupérer une cmd*/
	{
		if (isatty(STDIN_FILENO)) /*si le shell est en mode interactif*/
			write(STDOUT_FILENO, "$ ", 2); /*affiche le prompt*/

		read = getline(&line, &len, stdin); /*récupère la ligne de commande*/

		if (read == -1) /*si erreur de lecture*/
		{
			free(line); /*libère la mémoire*/
			if (!isatty(STDIN_FILENO)) /*si pas en mode interactif*/
				exit(0); /*sortie propre*/
			exit(0); /*sortie propre*/
		}

		for (i = 0; line[i]; i++) /*parcourt la ligne*/
		{
			/*si le caractère est un " ", un tab ou un retour à la ligne*/
			if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
				break; /*sort de la boucle*/
		}

		if (line[i] == '\0') /*si la ligne est vide*/
		{
			free(line); /*libère la mémoire*/
			line = NULL; /*réinitialise la ligne*/
			continue; /*continue la boucle*/
		}

		cleaner(line); /*nettoie la ligne*/

		args = split_line(line); /*découpe la ligne en mots*/
		if (!args) /*si pas de mots*/
		{
			cleanup(line, NULL); /*libère la mémoire*/
			line = NULL; /*réinitialise la ligne*/
			continue; /*continue la boucle*/
		}

		if (handle_exit(args)) /*si la commande est "exit" ?*/
		{
			cleanup(line, args); /*libère la mémoire*/
			exit(0); /*sortie propre*/
		}
		else if (strcmp(args[0], "env") == 0) /*si la commande est "env" ?*/
		{
			handle_env(); /*affiche les variables d'env*/
			cleanup(line, args); /*libère la mémoire*/
			line = NULL; /*réinitialise la ligne*/
		}
		else
		{
			execute_command(args, av); /*exécute la commande*/
			cleanup(line, args); /*libère la mémoire*/
			line = NULL; /*réinitialise la ligne*/
		}
	}
	return (0); /*retourne 0 pr indiquer que tout s'est bien passé*/
}

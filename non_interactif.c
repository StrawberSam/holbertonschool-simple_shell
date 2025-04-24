#include "main.h"

/**
 * handle_non_interactive - Gère le shell en mode non interactif
 * @av: Tableau contenant argv[0]
 */
void handle_non_interactive(char **av)
{
	char *line = NULL; /*récupère la commande*/
	size_t len = 0; /*récupère la length de la commande*/
	ssize_t read; /*récupère le nombre de caractères lus*/
	char **args; /*tableau pour stocker les mots de la commande*/

	read = getline(&line, &len, stdin); /*lit la ligne*/
	cleaner(line); /*nettoie le caractère de fin et si vide*/

	while (read != -1) /*tant que la lecture n'échoue pas*/
	{
		if (strchr(line, ' ') == NULL) /*si pas d'espace*/
		{
			execute_simple_command(line);  /* exécute la commande simple */
			exit(127);
		}

		args = split_line(line); /*découpe la ligne en mots*/
		if (!args || !args[0]) /*si pas de mots ou si args est NULL*/
		{
			cleanup(line, args);
			continue; /*continue la boucle*/
		}

		if (strcmp(args[0], "exit") == 0) /*si la commande est exit*/
		{
			cleanup(line, args); /*libère la mémoire*/
			exit(0); /*on quitte le programme*/
		}

		execute_command(args, av); /*exécute la commande*/
		cleanup(line, args); /*libère la mémoire*/
	}
	free(line); /*libère la mémoire de la ligne*/
}


#include "main.h"

/**
 * cleanup - Libère la mémoire allouée pour la ligne et les arguments
 * @line: Ligne lue
 * @args: Arguments de la commande
 */
void cleanup(char *line, char **args)
{
	free(line); /*libération de la mémoire de line*/
	free(args); /*libération de la mémoire de l'array de mots*/
}

/**
 * read_line - Lit une ligne depuis stdin ou quitte
 * @line: pointeur vers la ligne (sera allouée)
 * @len: pointeur vers la taille du buffer (sera ajustée)
 * Return: nombre d’octets lus
 */
ssize_t read_line(char **line, size_t *len)
{
	/*getline lit une ligne depuis stdin et la stocke dans line*/
	ssize_t read = getline(line, len, stdin);

	if (read == -1) /*si la lecture echoue*/
	{
		if (feof(stdin)) /*par ce qu'on a fait Ctrl+D */
			exit(0); /*on ferme le programme*/
		else
		{
			perror("getline"); /*affiche l'erreur*/
			exit(1); /*on quitte le programme*/
		}
	}
	return (read); /*retourne le nombre de caractères lus*/
}

/**
 * simple_command - Gère les commandes sans arguments
 * @line: commande simple
 */
void simple_command(char *line)
{
	char *simple_args[2]; /*tableau pour stocker la commande et NULL*/

	simple_args[0] = line; /*la première case du tableau contient la commande*/
	simple_args[1] = NULL; /*la deuxième case du tableau contient NULL*/

	if (access(line, X_OK) == 0) /*vérifie si la commande est exécutable*/
	{
		execve(line, simple_args, environ); /*exécute la commande*/
		perror(line); /*affiche l'erreur*/
		exit(127); /*on quitte le programme*/
	}
	else /*si la cmd n'est pas exécutable*/
	{
		perror(line); /*affiche l'erreur*/
		exit(127); /*on quitte le programme*/
	}
}

/**
 * execute_command_interactive - Exécute une commande avec fork + execve
 * @args: Tableau contenant la commande et ses arguments
 * @av: argv, pour afficher le nom du programme dans les erreurs
 */
void execute_command_interactive(char **args, char **av)
{
	pid_t pid; /*id du processus enfant*/
	char *full_path; /*chemin complet de la commande*/

	pid = fork(); /*crée un nouveau processus enfant*/
	if (pid == -1) /*si la création du processus enfant échoue*/
	{
		perror("fork"); /*affiche l'erreur*/
		exit(EXIT_FAILURE); /*on quitte le programme*/
}
	if (pid == 0) /*si la création du process enfant reussi*/
	{
		/*on vérifie si le 1er caractère est '/' ou '.'*/
		if (args[0][0] == '/' || args[0][0] == '.')
		{
			if (access(args[0], X_OK) == 0) /*si la commande est exécutable*/
				execve(args[0], args, environ); /*exécute la commande*/
			else /*sinon on affiche un message d'erreur*/
			{
				fprintf(stderr, "%s: No such file or directory\n", args[0]);
				exit(127); /*on quitte le programme*/
			}
		}
		else /*si pas '/' ou '.'*/
		{
			/*cherche le chemin de la commande*/
			full_path = find_command_in_path(args[0]);
			if (full_path) /*si le chemin de la commande est trouvé*/
			{
				execve(full_path, args, environ); /*exécute la commande*/
				free(full_path); /*libère la mémoire allouée pour le chemin*/
			}
			else /*sinon on affiche un message d'erreur*/
			{
				fprintf(stderr, "%s: 1: %s: not found\n", av[0], args[0]);
				exit(127); /*on quitte le programme*/
			}
		}
	}
	else
		wait(NULL); /*attend la fin du processus enfant*/
}

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

	read = read_line(&line, &len); /*lit la ligne*/

	while (read != -1) /*tant que la lecture n'échoue pas*/
	{
		cleaner(line); /*nettoie la ligne*/
		if (strchr(line, ' ') == NULL) /*si pas d'espace*/
			simple_command(line); /*gère la commande simple*/

		args = split_line(line); /*découpe la ligne en mots*/
		if (!args || !args[0]) /*si pas de mots ou si args est NULL*/
		{
			cleanup(line, args); /*libère la mémoire*/
			read = read_line(&line, &len); /*lit la ligne*/
			continue; /*continue la boucle*/
		}

		if (strcmp(args[0], "exit") == 0) /*si la commande est exit*/
		{
			cleanup(line, args); /*libère la mémoire*/
			exit(0); /*on quitte le programme*/
		}

		execute_command_interactive(args, av); /*exécute la commande*/
		cleanup(line, args); /*libère la mémoire*/
		line = NULL;  /*réinitialise la ligne*/
		len = 0; /*réinitialise la longueur de la ligne*/
		read = read_line(&line, &len); /*lit la ligne*/
	}
	free(line); /*libère la mémoire de la ligne*/
}

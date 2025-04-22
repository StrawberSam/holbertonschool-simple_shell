#include "main.h"

/**
 * cleaner - nettoyage du \n
 *
 * @line: string
 */

void cleaner(char *line)
{
	int i;

	if (line == NULL)/*Vérifie que line n'est pas NULL*/
	{
		return;/*Si line est NULL, on ne fait rien*/
	}

	for (i = 0; line[i] != '\0'; i++)
	{
		if (line[i] == '\n')
		{
			line[i] = '\0';
			return;
		}
	}
}

/**
 * main - affiche un prompt "$ ", attend une commande
 * de l'utilisateur, puis l'affiche.
 * Il s'arrête proprement à la fin de fichier (Ctrl+D).
 *
 * Return: Always 0 (Success)
 */
int get_line(void)
{
	/*pointeur vers chaine contenant la ligne de l'utilisateur*/
	char *line = NULL;
	/*taille initiale du buffer, getline ajuste la taille automatiquement*/
	size_t len = 0;
	/*stock le nombre de caractère lu par getline*/
	ssize_t read;

	/*permet une boucle infinie tant que l'utilisateur ne tape pas EOF*/
	printf("$ ");/*affiche prompt et attend la ligne de l'utilisateur*/
	read = getline(&line, &len, stdin);

	if (read == -1)
	{
/*L'utilisateur tape EOF. Fin de fichier détectée. Le programme se termine*/
		if (feof(stdin))
		{
			exit (0);
		}
		else/*Erreur de lecture*/
		{
			printf(": command not found.");
			return (-1);
		}
	}
	cleaner(line);/*appel fonction: nettoyage du \n et si line non valide*/

	free(line);/*free car getline*/
	return (0);
}

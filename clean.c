#include "main.h"

/**
 * cleanup - Libère la mémoire allouée pour la cmd et l'array de mots
 * @line: commande lue
 * @args: tableau de mots
 */
void cleanup(char *line, char **args)
{
	free(line); /*libération de la mémoire de la cmd*/
	free(args); /*libération de la mémoire de l'array de mots*/
}

/**
 * cleaner - nettoyage du \n
 *
 * @line: string (la commande)
 */
void cleaner(char *line)
{
	int i; /*variable de boucle par l'index*/

	if (line == NULL) /*Vérifie que cmd n'est pas NULL*/
	{
		return; /*Si cmd est NULL, on ne fait rien*/
	}
	/*on avance sur les char de la cmd jusqu'à la fin*/
	for (i = 0; line[i] != '\0'; i++)
	{
		if (line[i] == '\n') /*quand on arrive au charriot...*/
		{
			line[i] = '\0'; /*...on le remplace par le char de fin de string*/
			return; /*on renvoie la modification*/
		}
	}
}

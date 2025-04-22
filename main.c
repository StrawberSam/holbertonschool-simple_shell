#include "main.h"

/**
 * main - Fonction principale pour utiliser get_line()
 *
 * Return: 0 la fonction c'est bien passée
 */

extern char **environ;

int main(void)
{
	char *line = NULL; /*stock la string récuprée par getline()*/
	char **args; /*stock les tokens*/

	while (1)
	{
		line = get_line(); /*on récupère la string*/
		if (line == NULL) /*si la commande est vide*/
			continue; /*on attend une nouvelle commande*/

		args = split_line(line); /*on coupe line en mot*/

		/*si il y a un argument et on compare args[0] avec "exit" pour exit*/
		if (args[0] != NULL && strcmp(args[0], "exit") == 0)
		{
			cleanup(line, args); /*on libère tout (mémoire)*/
			exit(0); /*on ferme le programme*/
		}
		/*Nettoyage mémoire*/
		free(line);
		free(args);
	}
	return (0);
}

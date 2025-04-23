#include "main.h"

char *find_command_in_path(const char *cmd)
{
	char *path = getenv("PATH"); /*on récupère le PATH en entier*/
	/*array(s) pour récupérer les mots de la commande, une copie du path, et un array*/
	char *token, *path_copy, *full_path;
	struct stat st; /*structure pour stocker les informations de stat()*/
	size_t len; /*nb de caractères des buffers token et path_copy*/

	if (!path || !cmd) /*si le path ou la commande vaut NULL*/
		return (NULL); /*on retourne NULL*/

	path_copy = strdup(path); /*on fait une copie du path car on ne doit pas modifier le path d'origine*/
	token = strtok(path_copy, ":"); /*on dit quoi découper (path_copy) et où (:)*/

	while (token != NULL) /*on boucle dans le buffer de path découpé*/
	{
		/*on calcul le nb de caractère de token et cmd plus le caractère "/" et celui de fin (+2)*/
		len = strlen(token) + strlen(cmd) + 2;
		full_path = malloc(len); /*on alloue la mémoire du nouveau buffer*/
		if (full_path == NULL) /*si l'allocation échoue*/
		{
			free(path_copy); /*on libère la mémoire de path_copy*/
			return (NULL); /*et on retourne NULL*/
		}
		/*dans full_path on print un certain nb de char (len) dans le format "%s / %s" avec les strings token et cmd*/
		snprintf(full_path, len, "%s/%s", token, cmd);

		/*on vérifie si la commande est trouvée dans le PATH*/
		if (stat(full_path, &st) == 0)
		{
			free(path_copy); /*on peu libérer la mémoire de path_copy car plus besoin*/
			return (full_path); /*on retourne le path de la commande*/
		}

		free(full_path); /*on libère la mémoire pour pouvoir traiter une autre commande*/
		token = strtok(NULL, ":"); /*on découpe le path (voilà pourquoi on fait une copie du path)*/
	}

	free(path_copy); /*on peu libérer la mémoire de path_copy car plus besoin*/
	return (NULL); /*on renvoie NULL par ce que cmd pas trouvée dans le path*/
}

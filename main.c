#include "main.h"

/**
 * main - Fonction principale pour utiliser get_line()
 *
 * Return: 0 la fonction c'est bien passée
 */

int main(int ac, char **av)
{
	char *line = NULL; /*stock la string récuprée par getline()*/
	char **args; /*stock les tokens*/
	pid_t pid; /*stock l'id du process enfant*/
	char *full_path = NULL;
	extern char **environ;
	(void)ac;

	while (1)
	{
		/*permet une boucle infinie tant que l'utilisateur ne tape pas EOF*/
		if (isatty(STDIN_FILENO))/*interactive mode*/
		{
			printf("$ ");/*affiche prompt et attend la ligne de l'utilisateur*/
		}

		line = get_line(); /*on récupère la string*/
		if (line == NULL) /*si la commande est vide*/
			continue; /*on attend une nouvelle commande*/

		args = split_line(line); /*on coupe line en mot*/

		if (!args)
		{
			free(line);
			continue;
		}
		/*si il y a un argument et on compare args[0] avec "exit" pour exit*/
		if (args[0] != NULL && strcmp(args[0], "exit") == 0)
		{
			free(line);
			free(args);
			exit(0); /*on ferme le programme*/
		}
		if (args[0] != NULL && strcmp(args[0], "env") == 0)
		{
			int i = 0;

			while (environ[i])
			{
				printf("%s\n", environ[i]);
				i++;
			}
			free(line);
			free(args);
			continue; /* on passe au prompt suivant sans fork */
		}

		pid = fork(); /*on crée un nouvel enfant et on récupère son ID*/

      	if (pid == -1) /*si le fork de l'enfant échoue*/
      	{
        	perror("fork"); /*message d'erreur dans le terminal*/
        	free(line); /*on libère la mémoire*/
			free(args);
        	return (1); /*on renvoie 1*/
      	}
      	if (pid == 0) /* si le fork réussit */
      	{
        	/*on vérifie si la commande existe et si oui on la stock dans une variable*/
        	full_path = find_command_in_path(args[0]);

        	if (args[0] != NULL && (args[0][0] == '.'))
			{
				if (access(args[0], X_OK) == 0)
					execve(args[0], args, environ);
				else
				{
					perror("./shell");
					free(line);
					free(args);
					exit(EXIT_FAILURE);
				}
			}
			else if (args[0] != NULL && (args[0][0] == '/'))
			{
				execve(args[0], args, environ);
				perror("./shell");
				free(line);
				free(args);
				exit(EXIT_FAILURE);
			}
			else if (full_path != NULL)
			{
				execve(full_path, args, environ);
				perror(args[0]);
				free(full_path);
				free(line);
				free(args);
				exit(1);
			}
			else
				fprintf(stderr, "%s: 1: %s: not found\n", av[0], args[0]);
			
			free(line);
			free(args);
          	exit(127); /* on termine proprement le processus enfant avec un code d'erreur */
      	}
      	else
    		wait(NULL); /*le parent attend la fin de l'execution de l'enfant*/
	}
	/*Nettoyage mémoire*/
	free(line);
	free(args);
	line = NULL; /*on rénitialise la variable de stockage du tableau de char*/
	args = NULL;
	return (0);
}

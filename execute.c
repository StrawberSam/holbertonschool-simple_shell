#include "main.h"

/**
 * execute_command - Exécute une commande en utilisant fork et execve
 * @args: Tableau d'arguments de la commande
 * @av: Tableau contenant argv[0] (nom du programme)
 */

void execute_command(char **args, char **av)
{
    pid_t pid; /*id du process enfant*/
    char *full_path; /*chemin complet de la commande*/

    pid = fork(); /*Création d'un processus enfant*/
    if (pid == -1) /*Erreur de fork*/
    {
        perror("fork"); /*Affiche l'erreur*/
        return; /*Retourne à la fonction appelante*/
    }
    if (pid == 0) /*si la création de l'enfant réussie*/
    {
        /*on vérifie le premier caractère de la cmd*/
        if (args[0][0] == '/' || args[0][0] == '.')
        {
            if (access(args[0], X_OK) == 0) /*si la cmd est exécutable*/
                execve(args[0], args, environ); /*exécute la cmd*/
            perror(args[0]); /*affiche l'erreur*/
            exit(126); /*sortie avec erreur cmd trouvée mais non exec*/
        }

        full_path = _which(args[0]); /*cherche le chemin de la cmd*/
        if (full_path) /*si le chemin existe*/
        {
            execve(full_path, args, environ); /*exécute la cmd*/
            perror(args[0]); /*affiche l'erreur*/
            free(full_path); /*libère la mémoire*/
            exit(127); /*sortie avec erreur no found*/
        }
        else
        {
            /*si la cmd n'existe pas ont print une erreur*/
            fprintf(stderr, "%s: 1: %s: not found\n", av[0], args[0]);
            exit(127); /*sortie avec erreur no found*/
        }
    }
    else
        wait(NULL); /*attend la fin du process enfant*/
}

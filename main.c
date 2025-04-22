#include "main.h"

/**
 * main - Fonction principale pour utiliser get_line()
 * 
 * Return: 0 la fonction c'est bien passée
 */

extern char **environ;

int main(void)
{
    while(1) /*boucle infinie*/
    {
      get_line(); /*appel de la fonction get_line*/
    }
    return (0); /*la fonction c'est bien passée on return 0*/
}

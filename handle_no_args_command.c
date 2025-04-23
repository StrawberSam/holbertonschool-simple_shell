#include"main.h"

void handle_no_args_command(char *line)
{
	pid_t pid = fork();
	extern char **environ;
	char *args[2];
	args[0] = line;
	args[1] = NULL;

	if (pid == 0)
	{
		if (access(line, X_OK) == 0)
			execve(line, args, environ);

		else
		{
			fprintf(stderr, "%s: No such file or directory\n", line);
			exit(127);
		}
	}
	else
		wait(NULL);
}

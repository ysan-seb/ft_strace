#include "ft_strace.h"

static int		ft_strace_with_opt(char **av, char **env)
{
	(void)av;
	(void)env;
	return (0);
}

static int		ft_strace_without_opt(char **av, char **env)
{
	(void)av;
	(void)env;
	return (0);
}

static char		*take_cmd_path(char *cmd)
{
	// int			i;
	don't forgot sub !!!
	int			separator;
	char		cmd_path[PATH_MAX];
	char		*var_path;
	struct stat file_stat;
	
	// i = 0;
	separator = 0;
	if (stat(cmd, &file_stat) || !(var_path = getenv("PATH")))
		return (cmd);
	separator = strlen(strchr(var_path, ':'));
	printf("separator_pos : %d\n", separator);
	strncpy(cmd_path, var_path, separator);
	printf("res : %s\n", cmd_path);
	return (cmd);
}

int     ft_strace(char **av, t_opt opt, char **env)
{
	struct stat filestat;

	av[0] = take_cmd_path(av[0]);
	if (stat(av[0], &filestat) < 0) { 
		write(2, "ft_strace: Can't stat '", 23);
		write(2, av[0], strlen(av[0]));
		perror("'");
	}
	if (opt.c)
		ft_strace_with_opt(av, env);
	else
		ft_strace_without_opt(av, env);
	return (0);
}

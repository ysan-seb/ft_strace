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

int     ft_strace(char **av, t_opt opt, char **env)
{
	struct stat filestat;

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

#include "ft_strace.h"

static int		usage(void)
{
	printf("ft_strace: must have PROG [ARGS]\n");
	return (1);
}

int				main(int ac, char **av, char **env)
{
	if (ac < 2)
		return (usage());
	buffer.i = 0;
	ft_strace(av + 1, env);
	return (0);
}

#include "ft_strace.h"

static int		info(void)
{
	printf("ft_strace: must have PROG [ARGS]\n"
	"Try 'ft_strace -h' for more information.\n");
	return (1);
}

static int		usage(void)
{
	printf("usage: ft_strace [-h] PROG [ARGS]\n"
	"   or: ft_strace -c PROG [ARGS]\n"
	"\nOptions:\n   -c\tcount time, "
	"calls, and errors for each syscall and report summary\n"
	"   -h\tprint help message\n");
	return (0);
}

int				main(int ac, char **av, char **env)
{
	t_opt opt;

	if (ac < 2)
		return (info());
	memset(&opt, 0, sizeof(t_opt));
	if (!(av = parse_options(av, &opt)))
		return (1);
	if (opt.h)
		return (usage());
	ft_strace(av, opt, env);
	return (0);
}

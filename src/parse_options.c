#include "ft_strace.h"

static void	    *invalid_option(char c)
{
	printf("ft_strace: invalid option -- '%c'\n"
	"Try 'ft_strace -h' for more information.\n", c);
	return (NULL);
}

static int		valid_option(char c, t_opt *opt)
{
	if (c == 'c')
		opt->c = 1;
	else if (c == 'h')
		opt->h = 1;
	else	
		return (0);
	return (1);
}

char	**parse_options(char **av, t_opt *opt)
{
	int		i;
	int		j;
	
	i = 1;
	while (av[i] && av[i][0] == '-')
	{
		j = 1;
		while (av[i][j])
		{
			if (av[i][1] == '-')
			{
				if (av[i][2])
					return (invalid_option(av[i][1]));
				else
					return (av + i + 1);
			}
			else if (!valid_option(av[i][j], opt))
				return (invalid_option(av[i][j]));
			j++;
		} 
		i++;
	}
	return (av + i);
}
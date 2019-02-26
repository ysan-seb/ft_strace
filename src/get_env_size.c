#include "ft_strace.h"

int		get_env_size(char **env)
{
	int		i = 0;

	while (env[i])
		i++;
	return (i);
}
#include "ft_strace.h"

t_path      get_command_path(char *cmd)
{
	int                     i;
	int                     j;
	char            *path;
	t_path          cmd_path;

	i = 0;
	j = 0;
	if (access(cmd, F_OK| X_OK) == 0 || !(path = getenv("PATH"))) {
		strcpy(cmd_path.str, cmd);
		return (cmd_path);
	}
	while (path[i]) {
		if (path[i] == ':') {
			if (j + strlen(cmd) <= PATH_MAX) {
				if (path[i - 1] != '/')
					cmd_path.str[j] = '/';
				strcat(cmd_path.str, cmd);
				if (access(cmd_path.str, F_OK | X_OK) == 0)
					return (cmd_path);
			}
			memset(cmd_path.str, 0 , PATH_MAX);
			j = 0;
			i++;
		}
		cmd_path.str[j] = path[i];
		j++;
		i++;
	}
	if (j + strlen(cmd) <= PATH_MAX) {
		if (path[i] != '/')
			cmd_path.str[j] = '/';
		strcat(cmd_path.str, cmd);
		if (access(cmd_path.str, F_OK| X_OK) == 0)
			return (cmd_path);
	}
	strcpy(cmd_path.str, cmd);
	return (cmd_path);
}

int         padding(void)
{
	int             size;

	size = strlen(buffer.buff);
	if (size >= 40)
		return (0);
	else
		return (41 - size);
}

int		get_env_size(char **env)
{
	int             i = 0;

	while (env[i])
		i++;
	return (i);
}

static int	usage(void)
{
	printf("ft_strace: must have PROG [ARGS]\n");
	return (1);
}

int		main(int ac, char **av, char **env)
{
	if (ac < 2)
		return (usage());
	buffer.i = 0;
	ft_strace(av + 1, env);
	return (0);
}

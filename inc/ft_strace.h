#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>

typedef struct 	s_opt
{
	int 	c;
	int 	h;
}				t_opt;

char            **parse_options(char **av, t_opt *opt);
int				ft_strace(char **av, t_opt opt, char **env);
#ifndef STRACE_H
# define STRACE_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>
#include <linux/limits.h>

// #include <syscall.h>

typedef struct 	s_opt
{
	int 	c;
	int 	h;
}				t_opt;

typedef struct s_path
{
	char	str[PATH_MAX];
}				t_path;

char            **parse_options(char **av, t_opt *opt);
int				ft_strace(char **av, t_opt opt, char **env);
t_path			get_command_path(char *cmd);

#endif
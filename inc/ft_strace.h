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
#include <syscall.h>
#include <fcntl.h>
#include <signal.h>
#include <ctype.h>

#define gettid() syscall(SYS_gettid)
#define tgkill(pid, tid, sig) syscall(SYS_tgkill, pid, tid, sig)

typedef struct 	s_opt
{
	int 	c;
	int 	h;
}		t_opt;

typedef struct 	s_path
{
	char	str[PATH_MAX];
}		t_path;

typedef struct 	s_buffer
{
	int	i;
	char	buff[256];
}		t_buffer;

t_buffer	buffer;

char            **parse_options(char **av, t_opt *opt);
int		ft_strace(char **av, t_opt opt, char **env);
t_path		get_command_path(char *cmd);

int		get_env_size(char **env);

int		buffer_add_char(char c);
int		buffer_add_string(char *str);
void		buffer_flush(void);

int		padding(void);

int   		access_def(long param);
int    		mprotect_def(long param);
int			open_def(long param);
int			mmap_def_map(long param);
int     	mmap_def_prot(long param);
int			arch_prctl_def(long param);

int	syscall_param(pid_t child, struct user_regs_struct regs, char **av);
#endif

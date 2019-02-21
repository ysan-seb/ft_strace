#include "ft_strace.h"
#include "sysname.h"

static int		ft_strace_with_opt(char **av, char **env)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		execve(av[0], av, env);
	else
		wait(0);
	return (0);
}

static int		ft_strace_without_opt(char **av, char **env)
{
	int						status;
	pid_t					child;
	long					params[6];
	// struct user_regs_struct	regs;
	int					ret;
	(void)ret;
	child = fork();
	if (child == 0) {
		execve(av[0], av, env);
	}
	else {
		ptrace(PTRACE_SEIZE, child, 0, 0);
		ptrace(PTRACE_INTERRUPT, child, 0, 0);
		wait(&status);
		while(1) {
			// ptrace(PTRACE_GETREGS, child, 1, &regs);
			ret = ptrace(PTRACE_SYSCALL, child, 0, 0);
			waitpid(child, &status, 0);
			ret = ptrace(PTRACE_PEEKUSER, child, sizeof(long)*ORIG_RAX);
				printf("%s(", sysname[ret]);
			params[0] = ptrace(PTRACE_PEEKUSER, child, sizeof(long)*RDI);
				printf("%ld)", params[0]);
			// params[1] = ptrace(PTRACE_PEEKUSER, child, sizeof(long)*RSI);
			// 	printf("%ld, ", params[1]);
			// params[2] = ptrace(PTRACE_PEEKUSER, child, sizeof(long)*RDX);
			// 	printf("%ld, ", params[2]);
			// params[3] = ptrace(PTRACE_PEEKUSER, child, sizeof(long)*RCX);
			// 	printf("%ld, ", params[3]);
			// params[4] = ptrace(PTRACE_PEEKUSER, child, sizeof(long)*R8);
			// 	printf("%ld, ", params[4]);
			// params[5] = ptrace(PTRACE_PEEKUSER, child, sizeof(long)*R9);
			// 	printf("%ld)", params[5]);
			ret = ptrace(PTRACE_SYSCALL, child, 0, 0);
			waitpid(child, &status, 0);
			ret = ptrace(PTRACE_PEEKUSER, child, sizeof(long)*RAX);
				printf(" = %d\n", ret);
			if (WIFEXITED(status))
				break;
		}
		printf("+++ exited with 0 +++\n");
	}
	return (0);
}

int     ft_strace(char **av, t_opt opt, char **env)
{
	t_path		cmd_path;
	struct stat filestat;

	cmd_path = get_command_path(av[0]);
	av[0] = cmd_path.str;
	if (stat(av[0], &filestat) < 0) { 
		write(2, "ft_strace: Can't stat '", 23);
		write(2, av[0], strlen(av[0]));
		perror("'");
		return (1);
	}
	if (opt.c)
		ft_strace_with_opt(av, env);
	else
		ft_strace_without_opt(av, env);
	return (0);
}

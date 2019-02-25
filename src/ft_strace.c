#include "ft_strace.h"
#include "systable.h"
#include <signal.h>

int		get_env_size(char **env)
{
	int		i = 0;

	while (env[i])
		i++;
	return (i);
}

static int	print_param(pid_t child, struct user_regs_struct regs, char **av, char **env)
{
	int	i;
	char	c;
	int	nargs;
	long	param[6] = {regs.rdi, regs.rsi, regs.rdx, regs.rcx, regs.r8, regs.r9};

	i = 0;
	nargs = sys[regs.orig_rax].nargs;
	while (nargs > 0) {
		if ((void*)param[i] == av) {
			printf(" [\"%s\"]", av[0]);
		} else if ((void*)param[i] == env) {
			printf("[/* %d vars */]", get_env_size(env));
		} else if (sys[regs.orig_rax].typearg[i] == STR) {
			printf("\"");
			while ((c = (char)ptrace(PTRACE_PEEKTEXT, child, param[i], 0))) {
				printf("%c", c);
				param[i]++;
			}
			printf("\"");
		} else if (sys[regs.orig_rax].typearg[i] == PTR) {
			if (!param[i])
				printf("NULL");
			else
				printf("%p", (void*)param[i]);
		} else if (sys[regs.orig_rax].typearg[i] == HEX)
			!param[i] ? printf("0") : printf("0x%lx", param[i]);
		else
			printf("%ld", param[i]);
		nargs--;
		if (nargs > 0)
			printf(", ");
		i++;
	}
	return (0);
}

static int	print_return(struct user_regs_struct regs)
{
	if (sys[regs.orig_rax].ret == VOID)
		printf(") = ?\n");
	else if (sys[regs.orig_rax].ret == PTR)
		printf(") = %p\n", (void*)regs.rax);
	else if (sys[regs.orig_rax].ret == INT)
		printf(") = %d\n", (int)regs.rax);
	else
		printf(") = %lld\n", regs.rax);
	return (0);
}

static int	wait_syscall(pid_t child, int status)
{
	ptrace(PTRACE_SYSCALL, child, 0, 0);
	waitpid(child, &status, 0);
	if (WIFEXITED(status))
		return (1);
	return (0);
}

static int	ft_strace_without_opt(char **av, char **env)
{
	int					status;
	pid_t					child;
	struct user_regs_struct	regs;

	child = fork();
	if (child == 0) {
		execve(av[0], av, env);
	}
	else {
		ptrace(PTRACE_SEIZE, child, 0, 0);
		ptrace(PTRACE_INTERRUPT, child, 0, 0);
		wait(&status);
		while(1) {
			if (wait_syscall(child, status))
				break;
			ptrace(PTRACE_GETREGS, child, 1, &regs);
			printf("%s(", sys[regs.orig_rax].name);
			print_param(child, regs, av, env);
			if (wait_syscall(child, status))
				break;
			ptrace(PTRACE_GETREGS, child, 1, &regs);
			print_return(regs);
		}
		printf("+++ exited with 0 +++\n");
	}
	return (0);
}

int     ft_strace(char **av, t_opt opt, char **env)
{
	t_path		cmd_path;
	struct stat filestat;

	(void)opt;
	cmd_path = get_command_path(av[0]);
	av[0] = cmd_path.str;
	if (stat(av[0], &filestat) < 0) {
		write(2, "ft_strace: Can't stat '", 23);
		write(2, av[0], strlen(av[0]));
		perror("'");
		return (1);
	}
	ft_strace_without_opt(av, env);
	return (0);
}

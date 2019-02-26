#include "ft_strace.h"
#include "systable.h"

#include <syscall.h>
#include <signal.h>

int     param_type_str(pid_t child, long param)
{
	int 	i;
	char 	c;

	i = 0;
    buffer_add_char('"');
	while ((c = (char)ptrace(PTRACE_PEEKTEXT, child, param, 0))) {
		if (c == 9) {
			buffer_add_string("\\t");
			i++;
		} else if (c == 10) {
            buffer_add_string("\\n");
			i++;
		} else
            buffer_add_char(c);
		i++;
		if (i == 37)
			break;
		param++;
	}
	buffer_add_char('"');
	if (i == 37)
		buffer_add_string("...");
    return (0);
}

int     param_type_ptr(char tmp[64], long param[6], int i)
{
    if (!param[i])
    	buffer_add_string("NULL");
	else
    	sprintf(tmp, "%p", (void*)param[i]);
	return (0);
}

int     mmap_def_prot(long param)
{
    param == 0 ? buffer_add_string("PROT_NONE") : 0;
    param == 1 ? buffer_add_string("PROT_READ") : 0;
    param == 2 ? buffer_add_string("PROT_WRITE") : 0;
    param == 3 ? buffer_add_string("PROT_READ|PROT_WRITE") : 0;
    param == 4 ? buffer_add_string("PROT_EXEC") : 0;
    param == 5 ? buffer_add_string("PROT_READ|PROT_EXEC") : 0;
    param == 6 ? buffer_add_string("PROT_WRITE|PROT_EXEC") : 0;
    param == 7 ? buffer_add_string("PROT_READ|PROT_WRITE|PROT_EXEC") : 0;
    return (0);
}

int		mmap_def_map(long param)
{
	param & 0x01 ? buffer_add_string("MAP_SHARED") : 0;
	param & 0x02 ? buffer_add_string("MAP_PRIVATE") : 0;

	param & 0x10 ? buffer_add_string("|MAP_FIXED") : 0;
	param & 0x20 ? buffer_add_string("|MAP_ANONYMOUS") : 0;

	param & 0x0800 ? buffer_add_string("|MAP_DENYWRITE") : 0;
	return (0);
}

int		param_type_int(struct user_regs_struct regs, char tmp[64], long param[6], int i)
{
	if (regs.orig_rax == SYS_access && i == 1) {
		access_def(param[i]);
	} else if (regs.orig_rax == SYS_mprotect && i == 2) {
		mprotect_def(param[i]);
	} else if (regs.orig_rax == SYS_mmap && i == 2) {
		mmap_def_prot(param[i]);
	} else if (regs.orig_rax == SYS_mmap && i == 3) {
		mmap_def_map(param[i]);
	} else
			sprintf(tmp, "%d", (int)param[i]);
	return (0);
}

int     param_type(pid_t child, struct user_regs_struct regs, char tmp[64], char **av, int i)
{
	extern char **environ;
	long	param[6] = {regs.rdi, regs.rsi, regs.rdx, regs.r10, regs.r8, regs.r9};

    if ((void*)param[i] == av) {
		sprintf(tmp, "[\"%s\"]", av[0]);
	} else if ((void*)param[i] == environ) {
		sprintf(tmp, "[/* %d vars */]", get_env_size(environ));
	} else if (sys[regs.orig_rax].typearg[i] == STR) {
        param_type_str(child, param[i]);
	} else if (sys[regs.orig_rax].typearg[i] == PTR) {
		param_type_ptr(tmp, param, i);
	} else if (sys[regs.orig_rax].typearg[i] == HEX) {
		!param[i] ? buffer_add_char('0') : sprintf(tmp, "0x%lx", param[i]);
	} else if (sys[regs.orig_rax].typearg[i] == INT) {
		param_type_int(regs, tmp, param, i);
	} else
		sprintf(tmp, "%ld", param[i]);
	buffer_add_string(tmp);
    return (0);
}

static int	syscall_param(pid_t child, struct user_regs_struct regs, char **av)
{
	int		i;
	int		nargs;
	char	tmp[256];

	i = 0;
	nargs = sys[regs.orig_rax].nargs;
	sprintf(tmp, "%s(", sys[regs.orig_rax].name);
	buffer_add_string(tmp);
	memset(&tmp, 0, 64);
	while (nargs > 0) {
		param_type(child, regs, tmp, av, i);
		memset(&tmp, 0, 256);
		nargs--;
		if (nargs > 0)
			buffer_add_string(", ");
		i++;
	}
	buffer_add_string(") ");
	return (0);
}

static int	syscall_return(struct user_regs_struct regs)
{
	if (sys[regs.orig_rax].ret == VOID)
		printf("%s%*c ?\n", buffer.buff, padding(), '=');
	else if (sys[regs.orig_rax].ret == PTR)
		printf("%s%*c %p\n", buffer.buff, padding(), '=', (void*)regs.rax);
	else if (sys[regs.orig_rax].ret == INT)
		printf("%s%*c %d\n", buffer.buff, padding(), '=', (int)regs.rax);
	else
		printf("%s%*c %lld\n",  buffer.buff, padding(), '=', regs.rax);
	buffer_flush();
	return (0);
}

static int	ft_strace_without_opt(char **av, char **env)
{
	int						status;
	pid_t					child;
	struct user_regs_struct	regs;

	child = fork();
	if (child == 0) {
		execve(av[0], av, env);
	}
	else {
		ptrace(PTRACE_SEIZE, child, 0, 0);
		ptrace(PTRACE_INTERRUPT, child, 0, 0);
		ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);
		wait(&status);
		while(1) {

			ptrace(PTRACE_SYSCALL, child, 0, 0);
			waitpid(child, &status, 0);
			ptrace(PTRACE_GETREGS, child, 0, &regs);
			syscall_param(child, regs, av);
			

			ptrace(PTRACE_SYSCALL, child, 0, 0);
			waitpid(child, &status, 0);	
			ptrace(PTRACE_GETREGS, child, 0, &regs);
			syscall_return(regs);
			if (WIFEXITED(status))
				break;

		}
		printf("+++ exited with %d +++\n", 0);
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

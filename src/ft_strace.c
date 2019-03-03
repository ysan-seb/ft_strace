#include "ft_strace.h"
#include "systable.h"
#include "errno_def.h"

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
	} else if (regs.orig_rax == SYS_arch_prctl && i == 0) {
		arch_prctl_def(param[i]);
	} else if (regs.orig_rax == SYS_open && i == 1) {
		open_def(param[i]);
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

int	syscall_param(pid_t child, struct user_regs_struct regs, char **av)
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

int	syscall_return(struct user_regs_struct regs)
{
	char	tmp[256];
	char	err[256];
	long long ret;

	ret = regs.rax;
	if (ret < 0)
		ret = -1;
	if (sys[regs.orig_rax].ret == VOID)
		sprintf(tmp, "%*c ?", padding(), '=');
	else if (sys[regs.orig_rax].ret == PTR) {
		ret < 0 ? sprintf(tmp, "%*c %lld", padding(), '=', ret) : sprintf(tmp, "%*c %p", padding(), '=', (void*)ret);
	} else if (sys[regs.orig_rax].ret == INT)
		sprintf(tmp, "%*c %d", padding(), '=', (int)ret);
	else
		sprintf(tmp, "%*c %lld", padding(), '=', ret);
	buffer_add_string(tmp);
	if (ret < 0 && sys[regs.orig_rax].ret != VOID) {
		sprintf(err, " %s (%s)", errno_def[-regs.rax], strerror(-regs.rax));
		buffer_add_string(err);
	}
	printf("%s\n", buffer.buff);
	buffer_flush();
	return (0);
}

#include <elf.h>

int		filearch(char *file)
{
	Elf32_Ehdr header;

	FILE* f = fopen(file, "r");
  	if(f) {
    	fread(&header, 1, sizeof(header), f);
		if (header.e_type == ELFMAG0 && header.e_ident[EI_MAG1] == ELFMAG1 && header.e_ident[EI_MAG2] == ELFMAG2 && header.e_ident[EI_MAG3] == ELFMAG3 ) {
			if (header.e_ident[EI_CLASS] == ELFCLASS32)
				arch = 32;
			else if (header.e_ident[EI_CLASS] == ELFCLASS64)
				arch = 64;
			else {
				fclose(f);
				return (-1);
			}
		}
	} else
		return (-1);
	return (0);		
}

int     ft_strace(char **av, char **env)
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
	if (filearch(av[0]) == -1)
		exit (-1);
	ft_strace_without_opt(av, env);
	return (0);
}

// #include "ft_strace.h"

// static int     param_type_str(pid_t child, long param)
// {
// 	int 	i;
// 	char 	c;

// 	i = 0;
//     buffer_add_char('"');
// 	while ((c = (char)ptrace(PTRACE_PEEKTEXT, child, param, 0))) {
// 		if (c == 9) {
// 			buffer_add_string("\\t");
// 			i++;
// 		} else if (c == 10) {
//             buffer_add_string("\\n");
// 			i++;
// 		} else
//             buffer_add_char(c);
// 		i++;
// 		if (i == 37)
// 			break;
// 		param++;
// 	}
// 	buffer_add_char('"');
// 	if (i == 37)
// 		buffer_add_string("...");
//     return (0);
// }

// static int     param_type_ptr(char tmp[64], long param[6], int i)
// {
//     if (!param[i])
//     	buffer_add_string("NULL");
// 	else
//     	sprintf(tmp, "%p", (void*)param[i]);
// 	return (0);
// }

// static int		param_type_int(struct user_regs_struct regs, char tmp[64], long param[6], int i)
// {
// 	if (regs.orig_rax == SYS_access && i == 1) {
// 		access_def(param[i]);
// 	} else if (regs.orig_rax == SYS_mprotect && i == 2) {
// 		mprotect_def(param[i]);
// 	} else if (regs.orig_rax == SYS_mmap && i == 2) {
// 		mmap_def_prot(param[i]);
// 	} else if (regs.orig_rax == SYS_mmap && i == 3) {
// 		mmap_def_map(param[i]);
// 	} else if (regs.orig_rax == SYS_arch_prctl && i == 0) {
// 		arch_prctl_def(param[i]);
// 	} else if (regs.orig_rax == SYS_open && i == 1) {
// 		open_def(param[i]);
// 	} else
// 			sprintf(tmp, "%d", (int)param[i]);
// 	return (0);
// }

// static int     param_type(pid_t child, struct user_regs_struct regs, char tmp[64], char **av, int i)
// {
// 	extern char **environ;
// 	long	param[6] = {regs.rdi, regs.rsi, regs.rdx, regs.r10, regs.r8, regs.r9};

//     if ((void*)param[i] == av) {
// 		sprintf(tmp, "[\"%s\"]", av[0]);
// 	} else if ((void*)param[i] == environ) {
// 		sprintf(tmp, "[/* %d vars */]", get_env_size(environ));
// 	} else if (sys[regs.orig_rax].typearg[i] == STR) {
//         	param_type_str(child, param[i]);
// 	} else if (sys[regs.orig_rax].typearg[i] == PTR) {
// 		param_type_ptr(tmp, param, i);
// 	} else if (sys[regs.orig_rax].typearg[i] == HEX) {
// 		!param[i] ? buffer_add_char('0') : sprintf(tmp, "0x%lx", param[i]);
// 	} else if (sys[regs.orig_rax].typearg[i] == INT) {
// 		param_type_int(regs, tmp, param, i);
// 	} else
// 		sprintf(tmp, "%ld", param[i]);
// 	buffer_add_string(tmp);
//     return (0);
// }

// int	syscall_param(pid_t child, struct user_regs_struct regs, char **av)
// {
// 	int		i;
// 	int		nargs;
// 	char	tmp[256];

// 	i = 0;
// 	nargs = sys[regs.orig_rax].nargs;
// 	sprintf(tmp, "%s(", sys[regs.orig_rax].name);
// 	buffer_add_string(tmp);
// 	memset(&tmp, 0, 256);
// 	while (nargs > 0) {
// 		param_type(child, regs, tmp, av, i);
// 		memset(&tmp, 0, 256);
// 		nargs--;
// 		if (nargs > 0)
// 			buffer_add_string(", ");
// 		i++;
// 	}
// 	buffer_add_string(") ");
// 	return (0);
// }
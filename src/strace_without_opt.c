#include "ft_strace.h"
#include "signal_def.h"

static int	signal_behaviors(pid_t child, int status, sigset_t empty, sigset_t mask)
{
	siginfo_t	sig;
	int			sig_type;

	while (WIFSTOPPED(status) && (sig_type = WSTOPSIG(status)) != SIGTRAP) {
		ptrace(PTRACE_GETSIGINFO, child, 0, &sig);
		if (sig_type == SIGCHLD) {
			printf("\e[3;38;5;9m--- %s {si_signo=%s, si_code=%d, si_pid=%d, si_uid=%d, si_status=%d, si_utime=%ld, si_stime=%ld} ---\e[0m\n",
				sig_def[sig.si_signo], sig_def[sig.si_signo], sig.si_code, sig.si_pid, sig.si_uid, sig.si_status, sig.si_utime, sig.si_stime);
		} else if (sig_type == SIGSEGV) {
			printf("\e[3;38;5;9m--- %s {si_signo=%s, si_code=%d, si_pid=%d, si_uid=%d} ---\n+++ killed by SIGSEGV +++\e[0m\n",
					sig_def[sig.si_signo], sig_def[sig.si_signo], sig.si_code, sig.si_pid, sig.si_uid);
			tgkill(getpid(), gettid(), sig_type);
		} else if (sig_type == SIGWINCH) {
			printf("\e[3;38;5;9m--- %s {si_signo=%s, si_code=%d, si_pid=%d, si_uid=%d} ---\e[0m\n",
					sig_def[sig.si_signo], sig_def[sig.si_signo], sig.si_code, sig.si_pid, sig.si_uid);
		} else if (sig_type == SIGCONT) {
			printf("\e[3;38;5;9m--- %s {si_signo=%s, si_code=%d, si_pid=%d, si_uid=%d} ---\e[0m\n",
					sig_def[sig.si_signo], sig_def[sig.si_signo], sig.si_code, sig.si_pid, sig.si_uid);
		}
		ptrace(PTRACE_SYSCALL, child, 0, sig.si_signo);
		sigprocmask(SIG_SETMASK, &empty, NULL);
		waitpid(child, &status, 0);
		sigprocmask(SIG_BLOCK, &mask, NULL);
	}
	return (0);
}

static void	sigprocmask_init(sigset_t *mask, sigset_t *empty)
{
	sigemptyset(mask);
	sigemptyset(empty);
	sigaddset(mask, SIGHUP);
	sigaddset(mask, SIGINT);
	sigaddset(mask, SIGQUIT);
	sigaddset(mask, SIGPIPE);
	sigaddset(mask, SIGTERM);
}

int	ft_strace_without_opt(char **av, char **env)
{
	int						status;
	pid_t					child;
	struct user_regs_struct	regs;
	sigset_t 				mask;
	sigset_t 				empty;

	sigprocmask_init(&mask, &empty);
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
			sigprocmask(SIG_SETMASK, &empty, NULL);
			waitpid(child, &status, 0);
			sigprocmask(SIG_BLOCK, &mask, NULL);
			signal_behaviors(child, status, empty, mask);
			ptrace(PTRACE_GETREGS, child, 0, &regs);
			syscall_param(child, regs, av);
			if (WIFEXITED(status))
				break;
			ptrace(PTRACE_SYSCALL, child, 0, 0);
			sigprocmask(SIG_SETMASK, &empty, NULL);
			waitpid(child, &status, 0);
			sigprocmask(SIG_BLOCK, &mask, NULL);
			signal_behaviors(child, status, empty, mask);
			ptrace(PTRACE_GETREGS, child, 0, &regs);
			syscall_return(regs);
			if (WIFEXITED(status))
				break;
		}
		printf("+++ exited with %d +++\n", 0);
	}
	return (0);
}
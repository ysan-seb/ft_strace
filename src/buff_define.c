#include "ft_strace.h"

int     access_def(long param)
{
    int i;

    i = 0;
    if (param == F_OK)
		buffer_add_string("F_OK");
	i += param & R_OK ? 1 : 0;
	i += param & W_OK ? 2 : 0;
	i += param & X_OK ? 4 : 0;
    i == 1 ? buffer_add_string("R_OK") : 0;
    i == 2 ? buffer_add_string("W_OK") : 0;
    i == 3 ? buffer_add_string("R_OK|W_OK") : 0;
    i == 4 ? buffer_add_string("X_OK") : 0;
    i == 5 ? buffer_add_string("R_OK|X_OK") : 0;
    i == 6 ? buffer_add_string("W_OK|X_OK") : 0;
    i == 7 ? buffer_add_string("R_OK|W_OK|X_OK") : 0;
    return (0);
}

int		arch_prctl_def(long param)
{
	param == 4098 ? buffer_add_string("ARCH_SET_FS"): 0;
	param == 4099 ? buffer_add_string("ARCH_GET_FS"): 0;
	param == 4097 ? buffer_add_string("ARCH_SET_GS") : 0;
	param == 4100 ? buffer_add_string("ARCH_GET_GS"): 0;
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

int     mprotect_def(long param)
{
    param == 0 ? buffer_add_string("PROT_NONE") : 0;
    param == 1 ? buffer_add_string("PROT_READ") : 0;
    param == 2 ? buffer_add_string("PROT_WRITE") : 0;
    param == 4 ? buffer_add_string("PROT_EXEC") : 0;
    param == 0x01000000 ? buffer_add_string("PROT_GROWSUP") : 0;
    param == 0x02000000 ? buffer_add_string("PROT_GROWSDOWN") : 0;
    return (0);
}

int	open_def(long param)
{

	static long	o_def[] = {
		O_CREAT, O_EXCL, O_NOCTTY, O_TRUNC, O_APPEND, O_NONBLOCK, O_NDELAY, O_SYNC,
        	O_FSYNC, O_ASYNC, __O_LARGEFILE, __O_DIRECTORY, __O_NOFOLLOW, __O_CLOEXEC, __O_DIRECT, __O_NOATIME, __O_PATH, __O_DSYNC, __O_TMPFILE
	};
	static char     *o_string_def[] = {
                "O_CREAT", "O_EXCL", "O_NOCTTY", "O_TRUNC", "O_APPEND", "O_NONBLOCK", "O_NDELAY", "O_SYNC",
                "O_FSYNC", "O_ASYNC", "O_LARGEFILE", "O_DIRECTORY", "O_NOFOLLOW", "O_CLOEXEC", "O_DIRECT", "O_NOATIME", "O_PATH", "O_DSYNC", "O_TMPFILE"
	};

	int foo = 0;
	if (param & O_WRONLY) {
		foo = 1;
		buffer_add_string("O_WRONLY");
	} else if (param & O_RDWR) {
		foo = 1;
		buffer_add_string("O_RDWR");
	} else {
		foo = 1;
		buffer_add_string("O_RDONLY");
	}
	for (size_t i = 0; i < sizeof(o_def) / sizeof(o_def[0]); i++) {
		if (param & o_def[i]) {
			if (foo)
			  buffer_add_char('|');
			else
			  foo = 1;
			buffer_add_string(o_string_def[i]);
		}
	}
	return (0);
}

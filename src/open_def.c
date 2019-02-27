#include "ft_strace.h"

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

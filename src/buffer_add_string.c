#include "ft_strace.h"

int			buffer_add_string(char *str)
{
	int size;

	size = strlen(str);
	if (buffer.i + strlen(str) < 255) {
		strcpy(buffer.buff + buffer.i, str);
		buffer.i += size;
	} else
		return (0);
	return (1);
}
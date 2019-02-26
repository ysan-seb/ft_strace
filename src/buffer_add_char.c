#include "ft_strace.h"

int			buffer_add_char(char c)
{
	if (buffer.i < 255)
		buffer.buff[buffer.i++] = c;
	else
		return (0);
	return (1);
}
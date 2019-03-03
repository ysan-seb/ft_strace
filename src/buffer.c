#include "ft_strace.h"

void			buffer_flush(void)
{
	memset(&buffer, 0, sizeof(buffer));
}

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

int			buffer_add_char(char c)
{
	if (buffer.i < 255)
		buffer.buff[buffer.i++] = c;
	else
		return (0);
	return (1);
}
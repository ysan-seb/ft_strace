#include "ft_strace.h"

int	    padding(void)
{
	int		size;

	size = strlen(buffer.buff);
	if (size >= 40)
		return (0);
	else
		return (41 - size);
}

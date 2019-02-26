#include "ft_strace.h"

void			buffer_flush(void)
{
	memset(&buffer, 0, sizeof(buffer));
}
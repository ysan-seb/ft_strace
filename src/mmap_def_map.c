#include "ft_strace.h"

    int		mmap_def_map(long param)
{
	param & 0x01 ? buffer_add_string("MAP_SHARED") : 0;
	param & 0x02 ? buffer_add_string("MAP_PRIVATE") : 0;

	param & 0x10 ? buffer_add_string("|MAP_FIXED") : 0;
	param & 0x20 ? buffer_add_string("|MAP_ANONYMOUS") : 0;

	param & 0x0800 ? buffer_add_string("|MAP_DENYWRITE") : 0;
	return (0);
}
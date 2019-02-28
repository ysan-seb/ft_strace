#include "ft_strace.h"

int		arch_prctl_def(long param)
{
	param == 4098 ? buffer_add_string("ARCH_SET_FS"): 0;
	param == 4099 ? buffer_add_string("ARCH_GET_FS"): 0;
	param == 4097 ? buffer_add_string("ARCH_SET_GS") : 0;
	param == 4100 ? buffer_add_string("ARCH_GET_GS"): 0;
	return (0);
}
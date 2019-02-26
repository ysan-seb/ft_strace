#include "ft_strace.h"

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
#include "ft_strace.h"

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
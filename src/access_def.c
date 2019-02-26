#include "ft_strace.h"

int     access_def(long param)
{
    int i;

    i = 0;
    if (param == F_OK)
		buffer_add_string("F_OK");
	i += param & R_OK ? 1 : 0;
	i += param & W_OK ? 2 : 0;
	i += param & X_OK ? 4 : 0;
    i == 1 ? buffer_add_string("R_OK") : 0;
    i == 2 ? buffer_add_string("W_OK") : 0;
    i == 3 ? buffer_add_string("R_OK|W_OK") : 0;
    i == 4 ? buffer_add_string("X_OK") : 0;
    i == 5 ? buffer_add_string("R_OK|X_OK") : 0;
    i == 6 ? buffer_add_string("W_OK|X_OK") : 0;
    i == 7 ? buffer_add_string("R_OK|W_OK|X_OK") : 0;
    return (0);
}
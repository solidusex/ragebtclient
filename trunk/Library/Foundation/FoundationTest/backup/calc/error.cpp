#include "global.h"



void	calc_error(const char *str, size_t line)
{
		printf("%s : %d\n", str, line);
		exit(-1);
}
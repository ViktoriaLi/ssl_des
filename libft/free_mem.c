#include "includes/libft.h"

char **free_mem(char **field, int num)
{
	int i;

	i = 0;
	while (i < num)
	{
		free(field[i]);
		field[i] = NULL;
		i++;
	}
	free(field);
	field = NULL;
	return (field);
}

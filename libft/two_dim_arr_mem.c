/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   two_dim_arr_mem.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 16:23:14 by vlikhotk          #+#    #+#             */
/*   Updated: 2017/12/12 16:23:41 by vlikhotk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

void	two_dim_arr_mem(char ***field, int x, int y, char c)
{
	int i;
	int j;

	i = 0;
	*field = (char**)malloc(sizeof(char*) * (x + 1));
	(*field)[x] = NULL;
	while (i < x)
	{
		(*field)[i] = (char*)malloc(sizeof(char) * (y + 1));
		(*field)[i][y] = '\0';
		i++;
	}
	i = 0;
	while (i < x)
	{
		j = 0;
		while (j < y)
		{
			(*field)[i][j] = c;
			j++;
		}
		i++;
	}
}

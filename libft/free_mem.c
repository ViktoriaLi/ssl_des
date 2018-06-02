/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 13:33:48 by vlikhotk          #+#    #+#             */
/*   Updated: 2018/05/30 13:34:24 by vlikhotk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

void	free_mem(char **field, int num)
{
	int i;

	i = 0;
	while (field[i] && i < num)
	{
		ft_strdel(&field[i]);
		i++;
	}
	if (field)
		free(field);
	field = NULL;
}

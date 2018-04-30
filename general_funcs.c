/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional_funcs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/30 16:31:53 by vlikhotk          #+#    #+#             */
/*   Updated: 2018/04/30 16:31:55 by vlikhotk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int		find_symb(char *str, char flag, int len)
{
	int i;

	i = 0;
	while (i < len)
	{
		if (str[i] == flag)
			return (i);
		i++;
	}
	return (-1);
}

void clear_iterators(t_addition *iters)
{
	int i;

	i = 0;
	while (i < 4)
		(*iters).str1[i++] = 0;
	i = 0;
	while (i < 4)
		(*iters).str2[i++] = 0;
	(*iters).i = 0;
	(*iters).j = 0;
	(*iters).k = 0;
	(*iters).m = 0;
}

void flags_normalize(char *all_flags, t_args *params, int len)
{
  int i;
  int j;

  i = 0;
  j = 0;
  while (i < len)
  {
    j = 0;
    while ((*params).flags[j] && (*params).flags[j] != all_flags[i])
      j++;
    if ((*params).flags[j] != all_flags[i])
      (*params).flags[j] = all_flags[i];
    i++;
  }
  //printf("%s\n", (*params).flags);
}

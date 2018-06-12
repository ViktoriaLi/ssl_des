/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b64_flags_spaces.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 12:50:06 by vlikhotk          #+#    #+#             */
/*   Updated: 2018/06/12 12:50:09 by vlikhotk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int		b64_save_flags(int argc, char **all_flags, char **argv, t_args *params)
{
	t_addition	iters;

	clear_iterators(&iters);
	iters.i = 2;
	while (iters.i < argc)
	{
		if (ft_strcmp(argv[iters.i], "-e") == 0 || ft_strcmp(argv[iters.i],
		"-d") == 0 || ft_strcmp(argv[iters.i], "-i") == 0 ||
		ft_strcmp(argv[iters.i], "-o") == 0)
		{
			(*all_flags)[iters.j++] = argv[iters.i][1];
			if (argv[iters.i][1] == 'i' && argv[iters.i + 1])
				(*params).ifd = open(argv[++iters.i], O_RDONLY);
			if (argv[iters.i][1] == 'o')
				(*params).ofd = open(argv[++iters.i],
				O_WRONLY | O_APPEND | O_CREAT, 0777);
			iters.i++;
		}
		else
		{
			ft_printf("base64: invalid option -- %c\n", argv[iters.i][1]);
			return (0);
		}
	}
	return (1);
}

int		check_b64_flags(int argc, char **argv, t_args *params)
{
	char *all_flags;

	if (!(all_flags = (char *)malloc(argc - 1)))
		return (1);
	all_flags[argc - 1] = 0;
	if (!b64_save_flags(argc, &all_flags, argv, params))
		return (1);
	flags_normalize(all_flags, params, argc - 1);
	if ((find_symb((*params).flags, 'i', FLAG_LEN)) >= 0 &&
		(*params).ifd <= 0)
	{
		ft_printf("%s\n", "base64: option requires an argument -- i");
		return (1);
	}
	if ((find_symb((*params).flags, 'o', FLAG_LEN)) >= 0 &&
		(*params).ofd < 0)
	{
		ft_printf("%s\n", "base64: option requires an argument -- o");
		return (1);
	}
	return (0);
}

void	ignore_whitespaces(t_args *params, int fd, int ret, int j)
{
	t_addition iters;

	clear_iterators(&iters);
	while (iters.i < ret)
	{
		if ((*params).b64_buf[iters.i] != ' ')
			(*params).tmpad[j++] = (*params).b64_buf[iters.i++];
		else
			iters.i++;
	}
	(*params).tmpad[j] = 0;
	if (iters.i != ret && ret == 64)
	{
		while (iters.m < ret)
			(*params).b64_buf[iters.m++] = 0;
		iters.k = read(fd, params, ret - iters.j);
		ignore_whitespaces(params, fd, iters.k, j);
	}
	else
		return ;
}

void	b64_remove_whitespaces(t_args *params, int len, int fd, int ret)
{
	int i;

	i = 0;
	(*params).b64_buf[ret] = 0;
	if (len == 64)
	{
		ignore_whitespaces(params, fd, ret, 0);
		if ((*params).tmpad[0] != 0)
		{
			while ((*params).tmpad[i])
			{
				(*params).b64_buf[i] = (*params).tmpad[i];
				i++;
			}
			while (i < len)
				(*params).b64_buf[i++] = 0;
			i = 0;
			while (i < len)
				(*params).tmpad[i++] = 0;
		}
	}
}

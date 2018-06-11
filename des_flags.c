/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_flags.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/07 19:03:08 by vlikhotk          #+#    #+#             */
/*   Updated: 2018/06/07 19:03:11 by vlikhotk         ###   ########.fr       */
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
			if (argv[iters.i][1] == 'i')
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
		(*params).ifd < 0)
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

int	save_des_flags(char **all_flags, char **argv,
	t_args *params, t_addition *iters)
{
	(*all_flags)[(*iters).j] = argv[(*iters).i][1];
	if (argv[(*iters).i][1] == 'i')
		(*params).ifd = open(argv[++(*iters).i], O_RDONLY);
	else if (argv[(*iters).i][1] == 'o')
		(*params).ofd = open(argv[++(*iters).i], O_WRONLY
		| O_APPEND | O_CREAT, 0666);
	else if (argv[(*iters).i][1] == 'k')
	{
		(*params).des_key = argv[++(*iters).i];
		if (!(*params).des_key)
		{
			ft_printf("%s\n", "missing argument for -k");
			return (0);
		}
	}
	else if (argv[(*iters).i][1] == 'v')
	{
		(*params).vector16 = argv[++(*iters).i];
		if (!(*params).vector16)
		{
			ft_printf("%s\n", "missing IV argument for -v");
			return (0);
		}
	}
	(*iters).j++;
	(*iters).i++;
	(*all_flags)[(*iters).j] = 0;
	return (1);
}

int		if_correct_des_flag(char *flag)
{
	if (ft_strcmp(flag, "-e") == 0 || ft_strcmp(flag, "-d")
		== 0 || ft_strcmp(flag, "-i") == 0 || ft_strcmp(flag, "-o")
		== 0 || ft_strcmp(flag, "-a") == 0 || ft_strcmp(flag, "-k")
		== 0 || ft_strcmp(flag, "-v") == 0 || ft_strcmp(flag, "-s")
		== 0 || ft_strcmp(flag, "-p") == 0)
		return (1);
	return (0);
}

int		check_des_flags(int argc, char **argv, t_args *params,
	t_addition iters)
{
	char		*all_flags;

	if (!(all_flags = (char *)malloc(argc - 1)))
		return (1);
	while (iters.i < argc)
	{
		if (if_correct_des_flag(argv[iters.i]))
		{
			if (!save_des_flags(&all_flags, argv, params, &iters))
			{
				ft_strdel(&all_flags);
				return (1);
			}
		}
		else
		{
			ft_strdel(&all_flags);
			ft_printf("ft_ssl:Error: '%s' ", argv[iters.i]);
			ft_printf("%s\n", "is an invalid command.");
			return (1);
		}
	}
	flags_normalize(all_flags, params, argc - 1);
	if (((find_symb((*params).flags, 'i', FLAG_LEN)) >= 0
	&& (*params).ifd < 0) || ((find_symb((*params).flags,
		'o', FLAG_LEN)) >= 0 && (*params).ofd < 0))
	{
		ft_printf("usage: ft_ssl %s ", argv[1]);
		ft_printf("%s\n", "[-i file] [-v IV] [-k key] [-o file]");
		return (1);
	}
	if (find_symb((*params).flags, 'k', FLAG_LEN) < 0)
	{
		ft_printf("%s\n", "enter des-cbc password:");
		get_next_line(0, &params->des_key);
	}
	if (find_symb((*params).flags, 'v', FLAG_LEN) < 0
	&& ft_strcmp((*params).cipher, "des-cbc") == 0)
	{
		ft_printf("%s\n", "enter des-cbc IV:");
		get_next_line(0, &params->vector16);
	}
	return (0);
}

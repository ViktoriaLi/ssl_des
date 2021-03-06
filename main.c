/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/21 13:47:06 by vlikhotk          #+#    #+#             */
/*   Updated: 2018/06/07 18:53:07 by vlikhotk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int		if_no_args(int argc, char **argv)
{
	if (argc == 1)
	{
		ft_printf("%s", "usage: ft_ssl command ");
		ft_printf("%s\n", "[command opts] [command args]");
		return (0);
	}
	if (ft_strcmp(argv[1], "base64") != 0 && ft_strcmp(argv[1],
	"des") != 0 && ft_strcmp(argv[1], "des-ecb") != 0 &&
	ft_strcmp(argv[1], "des-cbc") != 0 && ft_strcmp(argv[1], "md5")
	!= 0 && ft_strcmp(argv[1],
	"sha256") != 0 && ft_strcmp(argv[1], "sha512") != 0)
	{
		ft_printf("ft_ssl: Error: %s is an invalid command.\n\n", argv[1]);
		ft_printf("%s\n", "Standard commands:\n\nMessage Digest commands:");
		ft_printf("%s", "md5\nsha256\nsha512\n\nCipher commands:\nbase64");
		ft_printf("%s\n", "\ndes\ndes-ecb\ndes-cbc");
		return (0);
	}
	return (1);
}

int		if_valid_args_des(int argc, char **argv, t_args *params)
{
	if ((ft_strcmp(argv[1], "base64") == 0) &&
	check_b64_flags(argc, argv, params) > 0)
		return (0);
	else if ((ft_strcmp(argv[1], "des") == 0) ||
	(ft_strcmp(argv[1], "des-ecb") == 0) ||
	(ft_strcmp(argv[1], "des-cbc") == 0))
	{
		(*params).cipher = argv[1];
		if (check_des_flags(argc, argv, params) > 0)
			return (0);
		if (!if_hex((*params).des_key, "non-hex digit\ninvalid hex key value"))
			return (0);
		if (ft_strcmp(argv[1], "des-cbc") == 0 && !if_hex((*params).vector16,
		"non-hex digit\ninvalid hex iv value"))
			return (0);
	}
	(*params).cipher = argv[1];
	return (1);
}

void	md5_main(t_args *params, t_addition *iters, int argc, char **argv)
{
	int	i;
	int	len;

	(*params).cipher = argv[1];
	if (ft_strcmp((*params).cipher, "sha512") == 0)
		len = 128;
	else
		len = 64;
	vectors_initiation(params, iters);
	reading_cases(params, iters, len);
	i = (*params).if_no_file;
	if (i)
		while (i < argc)
		{
			ft_printf("md5: %s: No such file or directory\n", argv[i]);
			i++;
		}
}

void	des_main(t_args *params)
{
	int fd;
	int len;

	fd = 0;
	len = 48;
	if (find_symb((*params).flags, 'i', FLAG_LEN) >= 0)
		fd = (*params).ifd;
	if ((ft_strcmp((*params).cipher, "des") == 0 ||
		ft_strcmp((*params).cipher, "des-ecb") == 0 ||
		ft_strcmp((*params).cipher, "des-cbc") == 0 ||
		ft_strcmp((*params).cipher, "base64") == 0) &&
		find_symb((*params).flags, 'd', FLAG_LEN) >= 0)
		len = 64;
	if (ft_strcmp((*params).cipher, "base64") == 0)
		base64_reading(fd, params, len);
	if (ft_strcmp((*params).cipher, "des") == 0 ||
		ft_strcmp((*params).cipher, "des-ecb") == 0 ||
		ft_strcmp((*params).cipher, "des-cbc") == 0)
		des_reading(fd, params, len);
}

int		main(int argc, char **argv)
{
	t_args		params;
	t_addition	iters;

	clear_struct(&params);
	clear_iterators(&iters);
	if (!if_no_args(argc, argv))
		return (0);
	if ((ft_strcmp(argv[1], "md5") == 0 || ft_strcmp(argv[1], "sha256")
	== 0 || ft_strcmp(argv[1], "sha512") == 0) &&
	!if_valid_args(argc, argv, &params, &iters))
		return (0);
	else if (!if_valid_args_des(argc, argv, &params))
		return (0);
	if (ft_strcmp(argv[1], "md5") == 0 || ft_strcmp(argv[1], "sha256")
	== 0 || ft_strcmp(argv[1], "sha512") == 0)
		md5_main(&params, &iters, argc, argv);
	else
		des_main(&params);
	if (params.ifd > 1)
		close(params.ifd);
	if (params.ofd > 1)
		close(params.ofd);
}

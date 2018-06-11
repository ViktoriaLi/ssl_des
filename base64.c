/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/28 17:16:44 by vlikhotk          #+#    #+#             */
/*   Updated: 2018/04/30 16:12:12 by vlikhotk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	print_or_save_res(t_args *params, unsigned char *str, int len)
{
	int i;

	i = 0;
	if (ft_strcmp((*params).cipher, "base64") == 0)
	{
		if ((find_symb((*params).flags, 'o', FLAG_LEN)) >= 0)
			write((*params).ofd, str, len);
		else
			write(1, str, len);
	}
	else
	{
		while (i < len)
			(*params).des_48_res[(*params).desad_count++] =
			str[i++];
	}
}

void	base64_dec(unsigned char *buf, t_args *params)
{
	t_addition				iters;

	clear_iterators(&iters);
	iters.str1[0] = find_symb(BAS, buf[0], 64);
	iters.str1[1] = find_symb(BAS, buf[1], 64);
	if (buf[2] != '=')
		iters.str1[2] = find_symb(BAS, buf[2], 64);
	if (buf[3] != '=')
		iters.str1[3] = find_symb(BAS, buf[3], 64);
	iters.str2[0] = (iters.str1[0] << 2) + (iters.str1[1] >> 4);
	iters.j = 1;
	if (buf[2] != '=')
	{
		iters.str2[1] = (iters.str1[1] << 4) + (iters.str1[2] >> 2);
		iters.j = 2;
	}
	if (buf[3] != '=')
	{
		iters.str2[2] = (iters.str1[2] << 6) + iters.str1[3];
		iters.j = 3;
	}
	print_or_save_res(params, iters.str2, iters.j);
}

void	base64_enc(unsigned char *buf, t_args *params, int j)
{
	unsigned char dest[4];

	dest[2] = '=';
	dest[3] = '=';
	dest[0] = BAS[buf[0] >> 2];
	dest[1] = BAS[((buf[0] & 3) << 4) + ((buf[1]) >> 4)];
	if (j > 1)
		dest[2] = BAS[((buf[1] & 15) << 2) + ((buf[2]) >> 6)];
	if (j > 2)
		dest[3] = BAS[buf[2] & 63];
	if ((find_symb((*params).flags, 'o', FLAG_LEN)) >= 0)
		write((*params).ofd, dest, 4);
	else
		write(1, dest, 4);
}

void	make_short_blocks(t_args *params, int ret, int len, unsigned char *str)
{
	t_addition				iters;
	static unsigned char	tmpb64d[4];

	clear_iterators(&iters);
	iters.k = ret;
	iters.m = ret;
	if (find_symb((*params).flags, 'd', FLAG_LEN) >= 0 &&
	ft_strcmp((*params).cipher, "base64") == 0)
		iters.k--;
	while (iters.i < iters.k)
	{
		iters.j = 0;
		while (iters.j < len && iters.i < iters.m)
			tmpb64d[iters.j++] = str[iters.i++];
		if (find_symb((*params).flags, 'd', FLAG_LEN) >= 0)
			base64_dec(tmpb64d, params);
		else
			base64_enc(tmpb64d, params, iters.j);
		iters.j = 0;
		while (iters.j < len)
			tmpb64d[iters.j++] = 0;
	}
}

void	ignore_whitespaces(t_args *params, int fd, int ret, int j)
{
	t_addition iters;

	clear_iterators(&iters);
	if (j == -1)
		j = 0;
	while ((*params).b64_buf[iters.i] != '\0' && iters.i < ret)
	{
		if ((*params).b64_buf[iters.i] != ' ')
		{
			(*params).tmpad[j++] = (*params).b64_buf[iters.i++];
		}
		else
			iters.i++;
	}
	(*params).tmpad[j++] = 0;
	if (iters.i != ret && ret == 64)
	{
		iters.m = 0;
		while (iters.m < ret)
			(*params).b64_buf[iters.m++] = 0;
		iters.k = read(fd, params, ret - iters.i);
		ignore_whitespaces(params, fd, iters.k, j);
	}
	else
		return ;
}

void	b64_remove_whitespaces(t_args *params, int len, int fd, int ret)
{
	int i;

	i = 0;
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

void	base64_reading(int fd, t_args *params, int len)
{
	t_addition				iters;

	clear_iterators(&iters);
	while ((iters.k = read(fd, &params->b64_buf, len)) > 0)
	{
		iters.i = 0;
		(*params).b64_buf[64] = 0;
		b64_remove_whitespaces(params, len, fd, iters.k);
		iters.k = ft_strlen((const char *)(*params).b64_buf);
		if (find_symb((*params).flags, 'd', FLAG_LEN) >= 0)
			make_short_blocks(params, iters.k, 4, (*params).b64_buf);
		else
			make_short_blocks(params, iters.k, 3, (*params).b64_buf);
		iters.i = 0;
		while (iters.i < iters.k)
			(*params).b64_buf[iters.i++] = 0;
		if (iters.k == 64 && (find_symb((*params).flags, 'd', FLAG_LEN)) < 0
			&& (find_symb((*params).flags, 'o', FLAG_LEN)) >= 0)
			write((*params).ofd, "\n", 1);
		else if (iters.k == 64 && (find_symb((*params).flags, 'd', FLAG_LEN))
		< 0 && (find_symb((*params).flags, 'o', FLAG_LEN)) < 0)
			write(1, "\n", 1);
	}
}

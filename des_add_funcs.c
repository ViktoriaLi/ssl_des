/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_add_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/10 15:56:44 by vlikhotk          #+#    #+#             */
/*   Updated: 2018/06/10 16:04:10 by vlikhotk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	to_binary(int **res, int nbr, int base)
{
	int	len;

	len = 3;
	if (!((*res) = (int *)malloc(sizeof(int) * 4)))
		return ;
	while (nbr > base - 2)
	{
		if ((nbr % base) < 10)
			(*res)[len--] = (nbr % base);
		nbr = nbr / base;
	}
	while (len >= 0)
		(*res)[len--] = 0;
}

void	vectors_preparing(t_args *params, int count, unsigned char save_res[])
{
	t_addition iters;

	clear_iterators(&iters);
	if (count == 1)
	{
		make_keys(&params->vector16, params, -1);
		while (iters.k < 8)
		{
			save_res[iters.k] = (*params).des_output[iters.k];
			iters.k++;
		}
		while (iters.i < 8)
		{
			(*params).b64_buf[iters.i] = (*params).buf[iters.i];
			iters.i++;
		}
		return ;
	}
	while (iters.k < 8)
	{
		save_res[iters.k] = (*params).b64_buf[iters.k];
		(*params).b64_buf[iters.k] = (*params).buf[iters.k];
		iters.k++;
	}
}

/*
** Divide block into left and right
*/

void	block_dividing(t_des_enc *des_params, t_args *params)
{
	t_addition iters;

	clear_iterators(&iters);
	while (iters.i < 4)
	{
		(*des_params).left[iters.i] = (*params).buf[iters.j];
		iters.i++;
		iters.j++;
	}
	while (iters.k < 4)
	{
		(*des_params).right[iters.k] = (*params).buf[iters.j];
		iters.k++;
		iters.j++;
	}
	while (iters.m < 4)
	{
		(*des_params).tmp[iters.m] = (*des_params).right[iters.m];
		iters.m++;
	}
}

void	make_64_bits(unsigned char exp_for_s[], unsigned char right48[],
t_des_enc *des_params)
{
	exp_for_s[0] = (right48[0] & 255) >> 2;
	exp_for_s[1] = (((right48[0] & 255) << 6) + (right48[1] >> 2)) >> 2;
	exp_for_s[2] = ((right48[1] & 255) << 4) + (right48[2] >> 4);
	exp_for_s[2] >>= 2;
	exp_for_s[3] = (right48[2] & 255) << 2;
	exp_for_s[3] >>= 2;
	exp_for_s[4] = (right48[3] & 255) >> 2;
	exp_for_s[5] = ((right48[3] & 255) << 6) + (right48[4] >> 2);
	exp_for_s[5] >>= 2;
	exp_for_s[6] = ((right48[4] & 255) << 4) + (right48[5] >> 4);
	exp_for_s[6] >>= 2;
	exp_for_s[7] = ((right48[5] & 255) << 2);
	exp_for_s[7] >>= 2;
	s_blocks_coords(des_params, exp_for_s);
}

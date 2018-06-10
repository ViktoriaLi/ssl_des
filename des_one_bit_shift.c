/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_one_bit_shift.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/10 16:23:22 by vlikhotk          #+#    #+#             */
/*   Updated: 2018/06/10 16:35:17 by vlikhotk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	set_one_right_bit(unsigned char key_res[], unsigned char key_56[],
int bit2)
{
	key_res[1] = ((key_56[0] & 255) << 7) + (key_56[1] >> 1);
	key_res[2] = ((key_56[1] & 255) << 7) + (key_56[2] >> 1);
	key_res[3] = ((key_56[2] & 255) << 7) + (key_56[3] >> 1);
	if (bit2)
		key_res[3] |= (1 << 3);
	else
		key_res[3] &= ~(1 << 3);
	key_res[4] = ((key_56[3] & 255) << 7) + (key_56[4] >> 1);
	key_res[5] = ((key_56[4] & 255) << 7) + (key_56[5] >> 1);
	key_res[6] = ((key_56[5] & 255) << 7) + (key_56[6] >> 1);
}

void	one_bit_shift_right(unsigned char key_56[], t_args *params)
{
	int				i;
	int				bit1;
	int				bit2;
	unsigned char	key_res[7];

	i = 0;
	bit1 = (1 << 4) & key_56[3];
	bit2 = (1 << 0) & key_56[6];
	key_res[0] = ((key_56[0] & 255) >> 1);
	if (bit1)
		key_res[0] |= (1 << 7);
	else
		key_res[0] &= ~(1 << 7);
	set_one_right_bit(key_res, key_56, bit2);
	while (i < 7)
	{
		(*params).key_res56[i] = key_res[i];
		i++;
	}
	finish_key_shift(key_res, params);
}

void	one_bit_shift_left(unsigned char key_56[], t_args *params)
{
	t_addition		iters;
	unsigned char	key_res[7];

	clear_iterators(&iters);
	iters.j = (1 << 7) & key_56[0];
	iters.k = (1 << 3) & key_56[3];
	key_res[0] = ((key_56[0] & 255) << 1) + (key_56[1] >> 7);
	key_res[1] = ((key_56[1] & 255) << 1) + (key_56[2] >> 7);
	key_res[2] = ((key_56[2] & 255) << 1) + (key_56[3] >> 7);
	key_res[3] = ((key_56[3] & 255) << 1) + (key_56[4] >> 7);
	key_res[3] &= ~(1 << 4);
	if (iters.j)
		key_res[3] |= (1 << 4);
	key_res[4] = ((key_56[4] & 255) << 1) + (key_56[5] >> 7);
	key_res[5] = ((key_56[5] & 255) << 1) + (key_56[6] >> 7);
	key_res[6] = ((key_56[6] & 255) << 1);
	key_res[6] &= ~(1 << 0);
	if (iters.k)
		key_res[6] |= (1 << 0);
	while (iters.i < 7)
	{
		(*params).key_res56[iters.i] = key_res[iters.i];
		iters.i++;
	}
	finish_key_shift(key_res, params);
}

void	bit_permutations(int max, const int table[],
	unsigned char key_56[], unsigned char *src)
{
	t_addition iters;

	clear_iterators(&iters);
	while (iters.i < max)
	{
		iters.j = 7;
		while (iters.j >= 0)
		{
			key_56[iters.i] &= ~(1 << iters.j);
			if (((1 << 0) & src[table[iters.k] / 8 - 1]))
				key_56[iters.i] |= (1 << iters.j);
			if ((table[iters.k] % 8) > 0)
			{
				key_56[iters.i] &= ~(1 << iters.j);
				if (((1 << (8 - (table[iters.k] % 8))) &
					src[table[iters.k] / 8]))
					key_56[iters.i] |= (1 << iters.j);
			}
			iters.k++;
			iters.j--;
		}
		iters.i++;
	}
}

void	start_shifting(t_args *params, const int shift_table_d[],
	unsigned char key_56[], int rounds)
{
	int i;

	i = 0;
	if (shift_table_d[rounds] == 0)
	{
		while (i < 7)
		{
			(*params).key_res56[i] = key_56[i];
			i++;
		}
		finish_key_shift(key_56, params);
	}
	if (shift_table_d[rounds] == 1)
		one_bit_shift_right(key_56, params);
	if (shift_table_d[rounds] == 2)
		two_bit_shift_right(key_56, params);
}

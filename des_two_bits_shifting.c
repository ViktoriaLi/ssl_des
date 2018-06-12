/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_two_bits_shifting.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/10 16:57:58 by vlikhotk          #+#    #+#             */
/*   Updated: 2018/06/10 16:59:36 by vlikhotk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

/*
** Step 6.3 Final key bits permutation
*/

void	finish_key_shift(unsigned char key_56[], t_args *params)
{
	const int key_finish[48] = {14, 17, 11, 24, 1, 5, 3, 28, 15, 6,
	21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52,
	31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32};

	bit_permutations(6, key_finish, (*params).key_res48, key_56);
}

void	set_two_right_bits(unsigned char key_res[], unsigned char key_56[],
int bit0, int bit1)
{
	key_res[1] = ((key_56[0] & 255) << 6) + (key_56[1] >> 2);
	key_res[2] = ((key_56[1] & 255) << 6) + (key_56[2] >> 2);
	key_res[3] = ((key_56[2] & 255) << 6) + (key_56[3] >> 2);
	if (bit0)
		key_res[3] |= (1 << 2);
	else
		key_res[3] &= ~(1 << 2);
	if (bit1)
		key_res[3] |= (1 << 3);
	else
		key_res[3] &= ~(1 << 3);
	key_res[4] = ((key_56[3] & 255) << 6) + (key_56[4] >> 2);
	key_res[5] = ((key_56[4] & 255) << 6) + (key_56[5] >> 2);
	key_res[6] = ((key_56[5] & 255) << 6) + (key_56[6] >> 2);
}

void	two_bit_shift_right(unsigned char key_56[], t_args *params)
{
	int						i;
	int						bit0;
	int						bit1;
	static unsigned char	key_res[7];

	i = 0;
	bit0 = (1 << 4) & key_56[3];
	bit1 = (1 << 5) & key_56[3];
	key_res[0] = ((key_56[0] & 255) >> 2);
	key_res[0] &= ~(1 << 6);
	if (bit0)
		key_res[0] |= (1 << 6);
	key_res[0] &= ~(1 << 7);
	if (bit1)
		key_res[0] |= (1 << 7);
	bit0 = (1 << 0) & key_56[6];
	bit1 = (1 << 1) & key_56[6];
	set_two_right_bits(key_res, key_56, bit0, bit1);
	while (i < 7)
	{
		(*params).key_res56[i] = key_res[i];
		i++;
	}
	finish_key_shift(key_res, params);
}

void	set_two_left_bits(unsigned char key_res[], unsigned char key_56[],
int bit0, int bit1)
{
	bit0 = (1 << 3) & key_56[3];
	bit1 = (1 << 2) & key_56[3];
	key_res[4] = ((key_56[4] & 255) << 2) + (key_56[5] >> 6);
	key_res[5] = ((key_56[5] & 255) << 2) + (key_56[6] >> 6);
	key_res[6] = ((key_56[6] & 255) << 2);
	if (bit0)
		key_res[6] |= (1 << 1);
	else
		key_res[6] &= ~(1 << 1);
	if (bit1)
		key_res[6] |= (1 << 0);
	else
		key_res[6] &= ~(1 << 0);
}

void	two_bit_shift_left(unsigned char key_56[], t_args *params)
{
	int				i;
	int				bit0;
	int				bit1;
	unsigned char	key_res[7];

	i = 0;
	bit0 = (1 << 7) & key_56[0];
	bit1 = (1 << 6) & key_56[0];
	key_res[0] = ((key_56[0] & 255) << 2) + (key_56[1] >> 6);
	key_res[1] = ((key_56[1] & 255) << 2) + (key_56[2] >> 6);
	key_res[2] = ((key_56[2] & 255) << 2) + (key_56[3] >> 6);
	key_res[3] = ((key_56[3] & 255) << 2) + (key_56[4] >> 6);
	key_res[3] &= ~(1 << 5);
	if (bit0)
		key_res[3] |= (1 << 5);
	key_res[3] &= ~(1 << 4);
	if (bit1)
		key_res[3] |= (1 << 4);
	set_two_left_bits(key_res, key_56, bit0, bit1);
	while (i < 7)
	{
		(*params).key_res56[i] = key_res[i];
		i++;
	}
	finish_key_shift(key_res, params);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_steps.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/10 16:40:44 by vlikhotk          #+#    #+#             */
/*   Updated: 2018/06/10 16:55:21 by vlikhotk         ###   ########.fr       */
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

/*
** Step 6.2 Make 56 bits key (remove each 8 bit)
** Step 6.3 Key cycle shift
*/

void	remove_8bits(unsigned char key_res[], t_args *params, int rounds)
{
	static unsigned char	key_56[7];
	const int				key_start[56] = {57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60,
	52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14,
	6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};
	const int				shift_table_e[16] = {1, 1, 2, 2, 2, 2, 2, 2,
	1, 2, 2, 2, 2, 2, 2, 1};
	const int				shift_table_d[16] = {0, 1, 2, 2, 2, 2, 2, 2,
	1, 2, 2, 2, 2, 2, 2, 1};

	if (rounds != -1)
	{
		bit_permutations(7, key_start, key_56, key_res);
		if (find_symb((*params).flags, 'd', FLAG_LEN) < 0)
		{
			if (shift_table_e[rounds] == 1)
				one_bit_shift_left(key_56, params);
			if (shift_table_e[rounds] == 2)
				two_bit_shift_left(key_56, params);
		}
		else
			start_shifting(params, shift_table_d, key_56, rounds);
	}
}

/*
** Step 6.1 Receive binary representation for hrxadecimal
** key and cut or lengthen to 64 bits
*/

void	make_keys(char **des_key, t_args *params, int rounds)
{
	t_addition		iters;
	unsigned char	key_res[KEY_LEN];

	clear_iterators(&iters);
	while ((*des_key)[iters.j] && iters.i < KEY_LEN)
	{
		if ((*des_key)[iters.j] >= 65 && (*des_key)[iters.j] <= 70)
			(*des_key)[iters.j] = (*des_key)[iters.j] - 7;
		else if ((*des_key)[iters.j] >= 97 && (*des_key)[iters.j] <= 102)
			(*des_key)[iters.j] = (*des_key)[iters.j] - 49;
		if ((*des_key)[iters.j + 1] >= 65 && (*des_key)[iters.j + 1] <= 70)
			(*des_key)[iters.j + 1] = (*des_key)[iters.j + 1] - 7;
		else if ((*des_key)[iters.j + 1] >= 97 && (*des_key)[iters.j + 1]
			<= 102)
			(*des_key)[iters.j + 1] = (*des_key)[iters.j + 1] - 49;
		key_res[iters.i++] = (((*des_key)[iters.j] - '0') * 16) +
		((*des_key)[iters.j + 1] - '0');
		iters.j += 2;
	}
	remove_8bits(key_res, params, rounds);
	while (iters.m < 8)
	{
		(*params).des_output[iters.m] = key_res[iters.m];
		iters.m++;
	}
}

/*
**initial bit permutation
*/

void	message_first_shift(t_args *params, t_des_enc *des_params,
	t_addition iters)
{
	static unsigned char	buf_res[DES_BLOCK];
	const int				m_start[64] = {58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6, 64, 56,
	48, 40, 32, 24, 16, 8, 57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35,
	27, 19, 11, 3, 61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31,
	23, 15, 7};

	if (ft_strcmp((*params).cipher, "des-cbc") == 0 &&
		find_symb((*params).flags, 'd', FLAG_LEN) < 0)
	{
		while (iters.i < 8)
		{
			(*params).buf[iters.i] ^= (*params).des_output[iters.i];
			iters.i++;
		}
	}
	bit_permutations(DES_BLOCK, m_start, buf_res, (*params).buf);
	while (iters.j < 8)
	{
		(*params).buf[iters.j] = buf_res[iters.j];
		iters.j++;
	}
	block_dividing(des_params, params);
}

void	start_keys_shifting(int rounds, t_args *params)
{
	const int shift_table_e[16] = {1, 1, 2, 2, 2, 2, 2, 2,
	1, 2, 2, 2, 2, 2, 2, 1};
	const int shift_table_d[16] = {0, 1, 2, 2, 2, 2, 2, 2,
	1, 2, 2, 2, 2, 2, 2, 1};

	if (rounds == 0)
		make_keys(&params->des_key, params, rounds);
	else
	{
		if (find_symb((*params).flags, 'd', FLAG_LEN) < 0)
		{
			if (shift_table_e[rounds] == 1)
				one_bit_shift_left((*params).key_res56, params);
			if (shift_table_e[rounds] == 2)
				two_bit_shift_left((*params).key_res56, params);
		}
		if (find_symb((*params).flags, 'd', FLAG_LEN) >= 0)
		{
			if (shift_table_d[rounds] == 1)
				one_bit_shift_right((*params).key_res56, params);
			if (shift_table_d[rounds] == 2)
				two_bit_shift_right((*params).key_res56, params);
		}
	}
}

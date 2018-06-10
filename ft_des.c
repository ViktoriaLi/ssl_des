/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_des.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/29 15:09:22 by vlikhotk          #+#    #+#             */
/*   Updated: 2018/06/06 17:51:52 by vlikhotk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void make_des_output(t_des_enc *des_params, t_args *params, int *l)
{
	t_addition iters;

	clear_iterators(&iters);
	make_64_bits_output(des_params, iters, params);
	remove_padding_and_merge_blocks(des_params, iters, params);
	if ((find_symb((*params).flags, 'a', FLAG_LEN)) < 0 ||
	(find_symb((*params).flags, 'a', FLAG_LEN) >= 0
	&& find_symb((*params).flags, 'd', FLAG_LEN) >= 0))
	{
		if ((find_symb((*params).flags, 'o', FLAG_LEN)) >= 0)
			write((*params).ofd, (*params).des_output, 8 - iters.m);
		else
			write(1, (*params).des_output, 8 - iters.m);
	}
	else if (find_symb((*params).flags, 'a', FLAG_LEN) >= 0 &&
	find_symb((*params).flags, 'd', FLAG_LEN) < 0)
	{
		while (iters.j < 8)
			(*params).des_48_res[(*l)++] = (*params).des_output[iters.j++];
		(*l) -= iters.m;
	}
}

void des_main_funcs(int rounds, t_args *params, t_des_enc *des_params)
{
	t_addition iters;
	const int p_shift[32] = {16,	7, 20, 21, 29, 12, 28, 17, 1, 15,	23,	26,	5, 18, 31, 10,\
  	2, 8, 24, 14, 32,	27,	3, 9, 19,	13,	30,	6, 22, 11, 4,	25};

	clear_iterators(&iters);
	start_keys_shifting(rounds, params);
	//Step 5. XOR key and right part
  while (iters.k < 6)
  {
    (*des_params).right48[iters.k] ^= (*params).key_res48[iters.k];
    iters.k++;
  }
	//Step 6. Make bits permutation with s-blocks
	//Step 6.1 Make 8 grops per 6 main bits
	make_64_bits((*des_params).exp_for_s, (*des_params).right48, des_params);

	//Step 6.3 Make s-blocks permutation
	s_blocks_proccessing(des_params);

	clear_iterators(&iters);
	//Step 7. Final bits permutation for right part
	bit_permutations(4, p_shift, (*des_params).right_f, (*des_params).right);
	//Step 8. XOR between left part and f_function result. Right part becomes new right part
	xor_left_right(des_params);
}

/*main des-encryption function */
void des_enc(t_args *params, int count, int *l)
{
	t_addition iters;
	t_des_enc des_params;
	int rounds;
	const int r_to_48[48] = {32,	1, 2,	3, 4,	5, 4,	5, 6,	7, 8,	9, 8,	9, 10, 11, 12, 13,\
  12,	13,	14,	15,	16,	17, 16,	17,	18,	19,	20,	21, 20,	21,	22,	23,	24,	25, 24,\
  25,	26,	27,	28,	29, 28,	29,	30,	31,	32,	1};

	rounds = 0;
	clear_iterators(&iters);
	if (ft_strcmp((*params).cipher, "des-cbc") == 0)
  	vectors_preparing(params, count, des_params.save_res);
	//Step 1. Make first bit permutation for message (8 bytes)
	if (ft_strcmp((*params).cipher, "des-cbc") == 0 &&
		find_symb((*params).flags, 'd', FLAG_LEN) < 0)
	{
		while (iters.i < 8)
		{
			(*params).buf[iters.i] ^= (*params).des_output[iters.i];
			iters.i++;
		}
	}
  message_first_shift(params);
	//Step 2. Message division into 2 parts
	block_dividing(&des_params, params);
  //start cycle with 16 rounds for message encryption (f-function)
	while (rounds < 16)
  {
	//Step 3. Make expansion of right part to 48 bits
	bit_permutations(6, r_to_48, des_params.right48, des_params.right);
	//Step 4. One key generation for current round
	des_main_funcs(rounds, params, &des_params);
	rounds++;
}
//Step 11. Make final encrypted output for message
make_des_output(&des_params, params, l);
}

void ignore_newline(t_args *params, int fd, int ret, int j)
{
	int i;
	int k;
	int l;
	int tmp;

	i = 0;
	l = 0;
	k = 0;
	tmp = j;
	if (j == -1)
		j = 0;
	while ((*params).des_48_read[i] != '\0' && i < ret)
	{
		if ((*params).des_48_read[i] != '\n' /*|| ((*params).des_48_read[i] == '\n' &&
	i == 63)*/)
	{
		(*params).tmpad[j++] = (*params).des_48_read[i++];

	}
		else
			i++;
	}
	if (i != ret)
	{
		l = 0;
		while (l < ret)
			(*params).des_48_read[l++] = 0;
		k = read(fd, params, ret - i);
		ignore_newline(params, fd, k, j);
	}
	else
		return ;
}

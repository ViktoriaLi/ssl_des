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

//Step 6.3 Final key bits permutation
void	finish_key_shift(unsigned char key_56[], t_args *params)
{
	const int key_finish[48] = {14,	17,	11,	24,	1, 5, 3, 28, 15, 6,
	21,	10,	23,	19,	12, 4, 26,	8,	16,	7,	27,	20,	13,	2,	41,	52,
	31,	37,	47,	55,	30,	40,	51,	45,	33,	48,	44,	49,	39, 56, 34, 53,
	46, 42, 50, 36, 29, 32};

	bit_permutations(6, key_finish, (*params).key_res48, key_56);
}

void set_two_right_bits(unsigned char key_res[], unsigned char key_56[],
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
	int				i;
	int				bit0;
	int				bit1;
	static unsigned char	key_res[7];

	i = 0;
	bit0 = (1 << 4) & key_56[3];
	bit1 = (1 << 5) & key_56[3];
	key_res[0] = ((key_56[0] & 255) >> 2);
	if (bit0)
		key_res[0] |= (1 << 6);
	else
		key_res[0] &= ~(1 << 6);
	if (bit1)
		key_res[0] |= (1 << 7);
	else
		key_res[0] &= ~(1 << 7);
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

void set_two_left_bits(unsigned char key_res[], unsigned char key_56[],
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
  if (bit0)
    key_res[3] |= (1 << 5);
  else
    key_res[3] &= ~(1 << 5);
  if (bit1)
    key_res[3] |= (1 << 4);
  else
    key_res[3] &= ~(1 << 4);
	set_two_left_bits(key_res, key_56, bit0, bit1);
	 while (i < 7)
	 {
	   (*params).key_res56[i] = key_res[i];
	   i++;
	 }
  finish_key_shift(key_res, params);
}

void set_one_right_bit(unsigned char key_res[], unsigned char key_56[],
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

void one_bit_shift_right(unsigned char key_56[], t_args *params)
{
	int i;
  int bit1;
	int bit2;
  unsigned char key_res[7];

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

void one_bit_shift_left(unsigned char key_56[], t_args *params)
{
	t_addition iters;

	clear_iterators(&iters);
  unsigned char key_res[7];

  iters.j = (1 << 7) & key_56[0];
	iters.k = (1 << 3) & key_56[3];
  key_res[0] = ((key_56[0] & 255) << 1) + (key_56[1] >> 7);
  key_res[1] = ((key_56[1] & 255) << 1) + (key_56[2] >> 7);
	key_res[2] = ((key_56[2] & 255) << 1) + (key_56[3] >> 7);
  key_res[3] = ((key_56[3] & 255) << 1) + (key_56[4] >> 7);
	if (iters.j)
		key_res[3] |= (1 << 4);
	else
		key_res[3] &= ~(1 << 4);
  key_res[4] = ((key_56[4] & 255) << 1) + (key_56[5] >> 7);
  key_res[5] = ((key_56[5] & 255) << 1) + (key_56[6] >> 7);
  key_res[6] = ((key_56[6] & 255) << 1);
  if (iters.k)
    key_res[6] |= (1 << 0);
  else
    key_res[6] &= ~(1 << 0);
	 while (iters.i < 7)
	 {
	   (*params).key_res56[iters.i] = key_res[iters.i];
	   iters.i++;
	 }
  finish_key_shift(key_res, params);
}

void	bit_permutations(int max, const int table[], unsigned char key_56[], unsigned char *src)
{
	t_addition iters;

	clear_iterators(&iters);
	while (iters.i < max)
	{
		iters.j = 7;
		while (iters.j >= 0)
		{
			if((table[iters.k] % 8) > 0)
			{
				if (((1 << (8 - (table[iters.k] % 8)))  &
					 src[table[iters.k] / 8]))
					key_56[iters.i] |= (1 << iters.j);
				else
					key_56[iters.i] &= ~(1 << iters.j);
			}
			else
			{
				if (((1 << 0) & src[table[iters.k] / 8 - 1]))
					key_56[iters.i] |= (1 << iters.j);
				else
				key_56[iters.i] &= ~(1 << iters.j);
			}
			iters.k++;
			iters.j--;
		}
		iters.i++;
	}
}

//Step 6.2 Make 56 bits key (remove each 8 bit)
void	remove_8bits(unsigned char key_res[], t_args *params, int rounds)
{
	int i;

	i = 0;
	static unsigned char key_56[7];
	const int key_start[56] = {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42,
	34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63,
	55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45,
	37, 29, 21, 13, 5, 28, 20, 12, 4};
	const int shift_table_e[16] = {1, 1, 2, 2,	2, 2,	2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
	const int shift_table_d[16] = {0, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
	bit_permutations(7, key_start, key_56, key_res);
	//Step 6.3 Key cycle shift
	if (find_symb((*params).flags, 'd', FLAG_LEN) < 0)
	{
		if (shift_table_e[rounds] == 1)
			one_bit_shift_left(key_56, params);
		if (shift_table_e[rounds] == 2)
			two_bit_shift_left(key_56, params);
	}
	else
	{
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
}

//Step 6.1 Receive binary representation for hrxadecimal key and cut or lengthen to 64 bits
void	make_keys(unsigned char **des_key, t_args *params, int rounds)
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
		else if ((*des_key)[iters.j + 1] >= 97 && (*des_key)[iters.j + 1] <= 102)
			(*des_key)[iters.j + 1] = (*des_key)[iters.j + 1] - 49;
		key_res[iters.i] = (((*des_key)[iters.j] - '0') * 16) + ((*des_key)[iters.j + 1] - '0');
		iters.i++;
		iters.j += 2;
	}
	if (rounds != -1)
		remove_8bits(key_res, params, rounds);
	else
		while (iters.m < 8)
		{
			(*params).des_output[iters.m] = key_res[iters.m];
			iters.m++;
		}
}

/*initial bit permutation*/
void message_first_shift(t_args *params)
{
	int i;

  static unsigned char buf_res[DES_BLOCK];
  const int m_start[64] = {58, 50, 42, 34, 26, 18, 10, 2,	60,	52,	44,	36,	28,	20,	12,	4,\
  62,	54,	46,	38,	30,	22,	14,	6, 64, 56,	48,	40,	32,	24,	16,	8, 57, 49, 41, 33,\
  25,	17,	9, 1,	59,	51,	43,	35,	27,	19,	11,	3, 61, 53,	45,	37,	29,	21,	13,	5, 63,\
  55,	47,	39,	31,	23,	15,	7};

  i = 0;
	bit_permutations(DES_BLOCK, m_start, buf_res, (*params).buf);
  while (i < 8)
  {
    (*params).buf[i] = buf_res[i];
    i++;
  }
}

/*main des-encryption function */
void des_enc(t_args *params, int count, int *l)
{
	t_addition iters;

  unsigned char left[4];
  unsigned char right[4];
	static unsigned char right_f[4];
	unsigned char tmp[4];
  static unsigned char right48[6];
	unsigned char exp_for_s[8];
	static unsigned char save_res[8];
	int string[2];
 	int column[4];
  int str_col[8][2];
  int s_output;
  int *four_bits;
	int rounds;

	rounds = 0;
  s_output = 0;
	four_bits = NULL;
	clear_iterators(&iters);
	//table for right part rotation
  const int r_to_48[48] = {32,	1, 2,	3, 4,	5, 4,	5, 6,	7, 8,	9, 8,	9, 10, 11, 12, 13,\
  12,	13,	14,	15,	16,	17, 16,	17,	18,	19,	20,	21, 20,	21,	22,	23,	24,	25, 24,\
  25,	26,	27,	28,	29, 28,	29,	30,	31,	32,	1};
	//8 s-blocks
  const int s_1[4][16] = {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
    {0, 15, 7, 4,	14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
    {4, 1, 14, 8, 13, 6, 2, 11, 15, 12,	9, 7, 3, 10, 5, 0},
    {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}};
	const int s_2[4][16] = {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
  {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
  {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
  {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}};
	const int s_3[4][16] = {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
  {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
  {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
  {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}};
	const int s_4[4][16] = {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
  {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
  {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
  {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}};
	const int s_5[4][16] = {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
  {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
  {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
  {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}};
	const int s_6[4][16] = {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
  {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
  {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
  {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}};
	const int s_7[4][16] = {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
  {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
  {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
  {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}};
	const int s_8[4][16] = {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
  {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
  {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
  {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}};
		//final right partshifting
	const int p_shift[32] = {16,	7, 20, 21, 29, 12, 28, 17, 1, 15,	23,	26,	5, 18, 31, 10,\
  	2, 8, 24, 14, 32,	27,	3, 9, 19,	13,	30,	6, 22, 11, 4,	25};
  //t_des_tables des_base;
	const int shift_table_e[16] = {1, 1, 2, 2,	2, 2,	2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
	const int shift_table_d[16] = {0, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
	const int m_end[64] = {40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, \
  38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, \
  52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26, \
  33, 1, 41, 9, 49, 17, 57, 25};
	if (ft_strcmp((*params).cipher, "des-cbc") == 0)
	{
		if (count == 1)
		{
			make_keys(&params->vector16, params, -1);
			iters.k = 0;
			while (iters.k < 8)
			{
				save_res[iters.k] = (*params).des_output[iters.k];
				iters.k++;
			}
			iters.k = 0;
			while (iters.k < 8)
			{
				(*params).b64_buf[iters.k] = (*params).buf[iters.k];
				iters.k++;
			}
		}
		else
		{
			iters.k = 0;
			while (iters.k < 8)
			{
				save_res[iters.k] = (*params).b64_buf[iters.k];
				iters.k++;
			}
			iters.k = 0;
			while (iters.k < 8)
			{
				(*params).b64_buf[iters.k] = (*params).buf[iters.k];
				iters.k++;
			}
		}
		iters.k = 0;
	}
	//Step 1. Make first bit permutation for message (8 bytes)
	if (ft_strcmp((*params).cipher, "des-cbc") == 0 && find_symb((*params).flags, 'd', FLAG_LEN) < 0)
	{
		while (iters.i < 8)
		{
			(*params).buf[iters.i] ^= (*params).des_output[iters.i];
			iters.i++;
		}
	}
  message_first_shift(params);
	//Step 2. Message division into 2 parts
	iters.i = 0;
  while (iters.i < 4)
  {
    left[iters.i] = (*params).buf[iters.j];
    iters.i++;
    iters.j++;
  }
  while (iters.k < 4)
  {
  	right[iters.k] = (*params).buf[iters.j];
    iters.k++;
    iters.j++;
  }
  while (iters.m < 4)
  {
		tmp[iters.m] = right[iters.m];
    iters.m++;
  }
  //start cycle with 16 rounds for message encryption (f-function)
	while (rounds < 16)
  {
	//Step 3. Make expansion of right part to 48 bits
	clear_iterators(&iters);
	bit_permutations(6, r_to_48, right48, right);
	//Step 4. One key generation for current round
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
	//Step 5. XOR key and right part
  while (iters.k < 6)
  {
    right48[iters.k] ^= (*params).key_res48[iters.k];
    iters.k++;
  }
	//Step 6. Make bits permutation with s-blocks
	//Step 6.1 Make 8 grops per 6 main bits
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
	//Step 6.2 Receive string and column numbers
	while (iters.j < 8)
	{
		if (((1 << 5) & exp_for_s[iters.j]))
			string[0] = 1;
		else
			string[0] = 0;
		if (((1 << 0) & exp_for_s[iters.j]))
			string[1] = 1;
		else
			string[1] = 0;
		if (((1 << 4) & exp_for_s[iters.j]))
			column[0] = 1;
		else
			column[0] = 0;
		if (((1 << 3) & exp_for_s[iters.j]))
			column[1] = 1;
		else
			column[1] = 0;
		if (((1 << 2) & exp_for_s[iters.j]))
			column[2] = 1;
		else
			column[2] = 0;
		if (((1 << 1) & exp_for_s[iters.j]))
			column[3] = 1;
		else
			column[3] = 0;
		str_col[iters.j][0] = string[0] * 2 + string[1];
		str_col[iters.j][1] = column[0] * 8 + column[1] * 4 + column[2] * 2 + column[3];
		iters.j++;
	}
	//Step 6.3 Make s-blocks permutation
  while (iters.m < 4)
  {
			// Receive new value 4 bits instead 6 bits
			if (iters.i == 0)
				s_output = s_1[str_col[iters.i][0]][str_col[iters.i][1]];
			else if (iters.i == 2)
				s_output = s_3[str_col[iters.i][0]][str_col[iters.i][1]];
			else if (iters.i == 4)
				s_output = s_5[str_col[iters.i][0]][str_col[iters.i][1]];
			else if (iters.i == 6)
				s_output = s_7[str_col[iters.i][0]][str_col[iters.i][1]];
			iters.i++;
			//Receive binary representation
			to_binary(&four_bits, s_output, 2);
			iters.k = 0;
			//assign new value to right block
			iters.j = 7;
			while (iters.k < 4)
			{
				if (four_bits[iters.k])
			    right[iters.m] |= (1 << iters.j);
			  else
			    right[iters.m] &= ~(1 << iters.j);
				iters.k++;
				iters.j--;
			}
			//repeat the same for next 4 bits
			if (iters.i == 1)
				s_output = s_2[str_col[iters.i][0]][str_col[iters.i][1]];
			else if (iters.i == 3)
				s_output = s_4[str_col[iters.i][0]][str_col[iters.i][1]];
			else if (iters.i == 5)
				s_output = s_6[str_col[iters.i][0]][str_col[iters.i][1]];
			else if (iters.i == 7)
				s_output = s_8[str_col[iters.i][0]][str_col[iters.i][1]];
			iters.i++;
			to_binary(&four_bits, s_output, 2);
			iters.k = 0;
			iters.j = 3;
			while (iters.k < 4)
			{
				if (four_bits[iters.k])
			    right[iters.m] |= (1 << iters.j);
			  else
			    right[iters.m] &= ~(1 << iters.j);
				iters.k++;
				iters.j--;
			}
    iters.m++;
  }
	clear_iterators(&iters);
	//Step 7. Final bits permutation for right part
	bit_permutations(4, p_shift, right_f, right);
	//Step 8. XOR between left part and f_function result. Right part becomes new right part
	while (iters.j < 4)
  {
    right[iters.j] = left[iters.j] ^ right_f[iters.j];
    iters.j++;
  }
	//Step 9. Left part becomes prev right part
	while (iters.k < 4)
	{
		left[iters.k] = tmp[iters.k];
		iters.k++;
	}
	//Step 10. Save right part for next round
	while (iters.m < 4)
	{
		tmp[iters.m] = right[iters.m];
		iters.m++;
	}
	rounds++;
}
//Step 11. Make final encrypted output for message
clear_iterators(&iters);
while (iters.i < 4)
{
	exp_for_s[iters.j] = right[iters.i];
	iters.j++;
	iters.i++;
}
iters.i = 0;
while (iters.i < 4)
{
	exp_for_s[iters.j] = left[iters.i];
	iters.j++;
	iters.i++;
}
bit_permutations(8, m_end, (*params).des_output, exp_for_s);
clear_iterators(&iters);
if (ft_strcmp((*params).cipher, "des-cbc") == 0 && find_symb((*params).flags, 'd', FLAG_LEN) >= 0)
{
	while (iters.m < 8)
	{
		(*params).des_output[iters.m] ^= save_res[iters.m];
		iters.m++;
	}
}
clear_iterators(&iters);
if (find_symb((*params).flags, 'd', FLAG_LEN) >= 0)
{
	if ((*params).des_output[7] > 0 && (*params).des_output[7] < 9)
	{
		iters.k = (*params).des_output[7];
		iters.m = (*params).des_output[7];
		iters.i = 7;
		while (iters.k > 0)
		{
			(*params).des_output[iters.i--] = 0;
			iters.k--;
		}
	}
}
if ((find_symb((*params).flags, 'a', FLAG_LEN)) < 0 || (find_symb((*params).flags, 'a', FLAG_LEN) >= 0
&& find_symb((*params).flags, 'd', FLAG_LEN) >= 0))
{
	if ((find_symb((*params).flags, 'o', FLAG_LEN)) >= 0)
		write((*params).ofd, (*params).des_output, 8 - iters.m);
	else
		write(1, (*params).des_output, 8 - iters.m);
}
else if (find_symb((*params).flags, 'a', FLAG_LEN) >= 0 && find_symb((*params).flags, 'd', FLAG_LEN) < 0)
{
	while (iters.j < 8)
		(*params).des_48_res[(*l)++] = (*params).des_output[iters.j++];
	(*l) -= iters.m;
}
}

void add_padding(t_args *params, int *ret, int len)
{
	t_addition				iters;

	clear_iterators(&iters);
	if ((find_symb((*params).flags, 'd', FLAG_LEN)) < 0 && (*ret % 8) == 0 && *ret != len)
	{
		iters.j = *ret;
		*ret += 8;
		while (iters.i < 8)
		{
			(*params).des_48_read[iters.j++] = 8;
			iters.i++;
		}
	}
	else if ((find_symb((*params).flags, 'd', FLAG_LEN)) < 0 && (*ret % 8) != 0 )
	{
		iters.j = *ret;
		iters.k = 8 - (*ret % 8);
		*ret += iters.k;
		while (iters.i < 8)
		{
				(*params).des_48_read[iters.j++] = iters.k;
				iters.i++;
		}
	}
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

void des_reading(int fd, t_args *params, int len)
{
  int i;
	int j;
	int k;
	int count;
  int ret;
	static unsigned char tmpb64[4];
	int l;

	i = 0;
	j = 0;
	k = 0;
  ret = 0;
	count = 0;
	l = 0;
			while ((ret = read(fd, params->des_48_read, len)) > 0)
			{
				if (len == 64)
				{
						ignore_newline(params, fd, ret, 0);
				if ((*params).tmpad[0] != 0)
				{
					i = 0;
					while ((*params).tmpad[i])
					{
						(*params).des_48_read[i] = (*params).tmpad[i];
						i++;
					}
					while (i < len)
						(*params).des_48_read[i++] = 0;
					i = 0;
					while (i < len)
						(*params).tmpad[i++] = 0;
				}
			}
				i = 0;
				l = 0;
				if (ret == len && (find_symb((*params).flags, 'd', FLAG_LEN)) < 0)
					(*params).if_full = 1;
					add_padding(params, &ret, len);
				if ((find_symb((*params).flags, 'a', FLAG_LEN)) >= 0 && find_symb((*params).flags, 'd', FLAG_LEN) >= 0)
				{
						(*params).desad_count = 0;
						make_short_blocks(params, ret, 4, (*params).des_48_read);
						i = 0;
						while (i < (*params).desad_count)
						{
							(*params).des_48_read[i] = (*params).des_48_res[i];
							i++;
						}
						ret = (*params).desad_count - (*params).desad_count % 8;
					}
				i = 0;
				while (i < ret)
				{
					j = 0;
					while (i < ret && j < 8)
						(*params).buf[j++] = (*params).des_48_read[i++];
					count++;
					des_enc(params, count, &l);
				}
					if ((find_symb((*params).flags, 'a', FLAG_LEN)) >= 0 && find_symb((*params).flags, 'd', FLAG_LEN) < 0)
					{
							i = 0;
							while (i < l)
							{
								j = 0;
								while (i < l && j < 3)
									tmpb64[j++] = (*params).des_48_res[i++];
								base64_enc(tmpb64, params, j);
								j = 0;
								while (j < 3)
									tmpb64[j++] = 0;
							}
							if (ret == 64 && (find_symb((*params).flags, 'o', FLAG_LEN)) >= 0)
					       write((*params).ofd, "\n", 1);
					    else if (ret == 48)
					       write(1, "\n", 1);
						i = 0;
						while (i < l)
							(*params).des_48_res[i++] = 0;
						l = 0;
					}
					i = 0;
					while (i < ret)
						(*params).des_48_read[i++] = 0;
			}
			if ((*params).if_full == 1 && (find_symb((*params).flags, 'd', FLAG_LEN)) < 0)
			{
				count++;
				j = 0;
				while (j < 8)
					(*params).buf[j++] = 8;
				des_enc(params, count, &l);
			}
}

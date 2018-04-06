/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_des.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/29 15:09:22 by vlikhotk          #+#    #+#             */
/*   Updated: 2018/03/29 15:09:25 by vlikhotk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <stdio.h>

void	to_binary(int **res, int nbr, unsigned int base)
{
	int i;
	int			len;
	int	d;
  int *tmp;

  d = nbr;
	len = 1;
	i = 0;
	while (d > base - 1)
	{
		d = d / base;
		len++;
	}
	i = len;
	if (!(tmp = (int *)malloc(sizeof(int) * len)))
		return ;
  if (!((*res) = (int *)malloc(sizeof(int) * 4)))
  		return ;
	while (len > 0)
	{
		if ((nbr % base) < 10)
			tmp[len--] = (nbr % base);
		nbr = nbr / base;
	}
  if (i < 4)
  {
		len = i;
    d = 3;
    while (len >= 0)
    {
      (*res)[d] = tmp[len];
      d--;
      len--;
    }
    while (d >= 0)
    {
		(*res)[d] = 0;
    }
  }
}

void finish_key_shift(unsigned char key_56[], t_args *params)
{
  int i;
  int j;
  int k;
  unsigned char key_48[6];
  const int key_finish[48] = {14,	17,	11,	24,	1,	5,	3,	28,	15,	6,	21,	10,	23,	19,	12,\
  	4, 26,	8,	16,	7,	27,	20,	13,	2,	41,	52,	31,	37,	47,	55,	30,	40,\
  	51,	45,	33,	48,	44,	49,	39, 56, 34, 53, 46, 42, 50, 36, 29, 32};

  i = 0;
  j = 0;
  k = 0;
  while (i < 6)
  {
    key_48[i] = 0;
    i++;
  }
  i = 0;
  while (i < 6)
  {
    j = 7;
    while (j >= 0)
    {
      key_48[i] |= ((1 << ((key_finish[k] % 8) - 1)) &
       key_56[(key_finish[k] - (key_finish[k] % 8)) / 8]) << j;
      k++;
      j--;
    }
    i++;
  }
  i = 0;
  while (i < 6)
  {
    (*params).key_res[i] = key_48[i];
    i++;
  }
  printf("finish key%s\n", (*params).key_res);
}

void two_bit_shift(unsigned char key_56[], t_args *params)
{
  int bit0;
  int bit1;
  unsigned char key_res[7];

  bit0 = 0;
  bit1 = 0;
  bit0 = key_56[0] >> 7;
  bit1 = (key_56[0] << 1) + (key_56[0] >> 6);
  key_res[0] = ((key_56[0] & 252) << 2) + (key_56[1] >> 6);
  key_res[1] = ((key_56[1] & 252) << 2) + (key_56[2] >> 6);
  key_res[2] = ((key_56[2] & 252) << 2) + (key_56[3] >> 6);
  key_res[3] = ((key_56[3] & 252) << 2);
  if (bit0 == 1)
    key_res[3] |= (1 << 6);
  else
    key_res[3] &= ~(1 << 6);
  if (bit1 == 1)
    key_res[3] |= (1 << 5);
  else
    key_res[3] &= ~(1 << 5);
  bit0 = ( key_56[3] >> 3 ) + (key_56[3] << 4);
  bit1 = ( key_56[3] >> 2 ) + (key_56[3] << 5);
  key_res[4] = ((key_56[4] & 252) << 2) + (key_56[5] >> 6);
  key_res[5] = ((key_56[5] & 252) << 2) + (key_56[6] >> 6);
  key_res[6] = ((key_56[6] & 252) << 2);
  if (bit0 == 1)
    key_res[6] |= (1 << 1);
  else
    key_res[6] &= ~(1 << 1);
  if (bit1 == 1)
    key_res[6] |= (1 << 0);
  else
    key_res[6] &= ~(1 << 0);
  finish_key_shift(key_res, params);
}

void one_bit_shift(unsigned char key_56[], t_args *params)
{
  int bit;
  unsigned char key_res[7];

  bit = 0;
  bit = key_56[0] >> 7;
  key_res[0] = ((key_56[0] & 254) << 1) + (key_56[1] >> 7);
  key_res[1] = ((key_56[1] & 254) << 1) + (key_56[2] >> 7);
  key_res[2] = ((key_56[2] & 254) << 1) + (key_56[3] >> 7);
  key_res[3] = ((key_56[3] & 254) << 1);
  if (bit == 1)
    key_res[3] |= (1 << 5);
  else
    key_res[3] &= ~(1 << 5);
  bit =( key_56[3] >> 3 ) + (key_56[3] << 4);
  key_res[4] = ((key_56[4] & 254) << 1) + (key_56[5] >> 7);
  key_res[5] = ((key_56[5] & 254) << 1) + (key_56[6] >> 7);
  key_res[6] = ((key_56[6] & 254) << 1);
  if (bit == 1)
    key_res[6] |= (1 << 0);
  else
    key_res[6] &= ~(1 << 0);
  printf("KEY after 1 bit shift%s\n", key_res);
  finish_key_shift(key_res, params);
}

void remove_8bits(unsigned char key_res[], t_args *params, int rounds)
{
  int i;
  int j;
  int k;
  unsigned char key_56[7];
  const int key_start[56] = {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2,\
  	59, 51, 43, 35, 27,	19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7,\
  	62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};
	const int shift_table_e[16] = {1, 1, 2, 2,	2, 2,	2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
  i = 0;
  j = 0;
  k = 0;
  while (i < 7)
  {
    key_56[i] = 0;
    i++;
  }
  i = 0;
  /*key_56[0] = ((key_res[0] & 254) << 1) + (key_res[1] >> 7);
  key_56[1] = ((key_res[1] & (254 << 2)) << 1) + (key_res[2] >> 6);
  key_56[2] = ((key_res[2] & (254 << 3)) << 2) + (key_res[3] >> 5);
  key_56[3] = ((key_res[3] & (254 << 4)) << 3) + (key_res[4] >> 4);
  key_56[4] = ((key_res[4] & (254 << 5)) << 4) + (key_res[5] >> 3);
  key_56[5] = ((key_res[5] & (254 << 6)) << 5) + (key_res[6] >> 2);
  key_56[6] = ((key_res[6] & (254 << 7)) << 6) + (key_res[7] >> 1);*/
  while (i < 7)
  {
    j = 7;
    while (j >= 0)
    {
      key_56[i] |= ((1 << ((key_start[k] % 8) - 1))  &
        key_res[(key_start[k] - (key_start[k] % 8)) / 8]) << j;
      k++;
      j--;
    }
    i++;
  }
  printf("KEY 56 bits%s\n", key_56);
	if (shift_table_e[rounds] == 1)
  	one_bit_shift(key_56, params);
	if (shift_table_e[rounds] == 2)
	 	two_bit_shift(key_56, params);
 // first_key_shift(key_56);
}

void make_keys(t_args *params, int rounds)
{
  int i;
  int j;
  unsigned char key_res[KEY_LEN];

  i = 0;
  j = 0;
  printf("Original key%s\n", (*params).des_key);
  while (i < KEY_LEN)
    key_res[i++] = 0;
  //key_res[0] = ( * 16) + (tmp[1] - '0');
  i = 0;
  while (i < KEY_LEN && (*params).des_key[j])
  {
    if ((*params).des_key[j] >= 65 && (*params).des_key[j] <= 70)
    {
      (*params).des_key[j] = (*params).des_key[j] - 7;
    }
    else if ((*params).des_key[j] >= 97 && (*params).des_key[j] <= 102)
    {
      (*params).des_key[j] = (*params).des_key[j] - 49;
    }
    if ((*params).des_key[j + 1] >= 65 && (*params).des_key[j + 1] <= 70)
    {
      (*params).des_key[j + 1] = (*params).des_key[j + 1] - 7;
    }
    else if ((*params).des_key[j + 1] >= 97 && (*params).des_key[j + 1] <= 102)
    {
      (*params).des_key[j + 1] = (*params).des_key[j + 1] - 49;
    }
    key_res[i] = (((*params).des_key[j] - '0') * 16) + ((*params).des_key[j + 1] - '0');
    i++;
    j += 2;
  }
  printf("64 bits key%s\n", key_res);
  remove_8bits(key_res, params, rounds);
  //printf("%d\n", key_res[0]);
}


/*initial bit permutation*/
void message_first_shift(t_args *params)
{
  int i;
  int j;
  int k;
  unsigned char buf_res[DES_BLOCK];
  const int m_start[64] = {58, 50, 42, 34, 26, 18, 10, 2,	60,	52,	44,	36,	28,	20,	12,	4,\
  62,	54,	46,	38,	30,	22,	14,	6, 64, 56,	48,	40,	32,	24,	16,	8, 57, 49, 41, 33,\
  25,	17,	9, 1,	59,	51,	43,	35,	27,	19,	11,	3, 61, 53,	45,	37,	29,	21,	13,	5, 63,\
  55,	47,	39,	31,	23,	15,	7};

  i = 0;
  j = 0;
  k = 0;
  while (i < DES_BLOCK)
    buf_res[i++] = 0;
  i = 0;
  while (i < DES_BLOCK)
  {
    j = 7;
    while (j >= 0)
    {
      //printf("d%d\n", ((m_start[k] % 8) - 1));
      //printf("c%d\n", (*params).buf[(m_start[k] - (m_start[k] % 8)) / 8]);
      if ((1 << ((m_start[k] % 8) - 1)) &
      (*params).buf[(m_start[k] - (m_start[k] % 8)) / 8])
        buf_res[i] |= ((1 << ((m_start[k] % 8) - 1)) &
        (*params).buf[(m_start[k] - (m_start[k] % 8)) / 8]) << j;
      else
        buf_res[i] &= (~(1 << ((m_start[k] % 8) - 1)) &
        (*params).buf[(m_start[k] - (m_start[k] % 8)) / 8]) << j;
      k++;
      j--;
    }
    i++;
  }
  i = 0;
  printf("first shift%s\n", buf_res);
  while (i < 8)
  {
    (*params).buf[i] = buf_res[i];
    i++;
  }
}

void des_dec(t_args *params)
{

}

/*void fill_des_tables(t_des_tables *des_base)
{
  (*des_base).m_start = {58, 50, 42, 34, 26, 18, 10, 2,	60,	52,	44,	36,	28	20,	12,	4,\
  62,	54,	46,	38,	30,	22,	14,	6, 64, 56,	48,	40,	32,	24,	16,	8, 57, 49, 41, 33,\
  25,	17,	9, 1,	59,	51,	43,	35,	27,	19,	11,	3, 61, 53,	45,	37,	29,	21,	13,	5, 63,\
  55,	47,	39,	31,	23,	15,	7};
	(*des_base).m_end = {40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, \
  38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, \
  52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26, \
  33, 1, 41, 9, 49, 17, 57, 25};
	(*des_base).r_to_48 = {32,	1, 2,	3, 4,	5, 4,	5, 6,	7, 8,	9, 8,	9, 10, 11, 12, 13\
  12,	13,	14,	15,	16,	17, 16,	17,	18,	19,	20,	21, 20,	21,	22,	23,	24,	25, 24,\
  25,	26,	27,	28,	29, 28,	29,	30,	31,	32,	1};
	(*des_base).key_start = {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2,\
  	59, 51, 43, 35, 27,	19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7,\
  	62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};
  (*des_base).key_finish = {14,	17,	11,	24,	1,	5,	3,	28,	15,	6,	21,	10,	23,	19,	12,\
  	4, 26,	8,	16,	7,	27,	20,	13,	2,	41,	52,	31,	37,	47,	55,	30,	40,\
  	51,	45,	33,	48,	44,	49,	39, 56, 34, 53, 46, 42, 50, 36, 29, 32};
	(*des_base).shift_table_e = {1, 1, 2, 2,	2, 2,	2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
	(*des_base).shift_table_d = {0, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
	(*des_base).s_1 = {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 0, 15, 7, 4,\
  	14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8, 4, 1, 14, 8, 13, 6, 2, 11, 15, 12,\
  	9, 7, 3, 10, 5, 0, 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13};
	(*des_base).s_2 = {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, 3, 13, 4, 7,\
  	15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5, 0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12,\
  	6, 9, 3, 2, 15, 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9};
	(*des_base).s_3 = {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8, 13, 7, 0, 9,\
  	3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1, 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2,\
  	12, 5, 10, 14, 7, 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12};
	(*des_base).s_4 = {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, 13, 8, 11, 5,\
  	6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9, 10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3,\
  	14, 5, 2, 8, 4, 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14};
	(*des_base).s_5 = {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9, 14, 11, 2, 12,\
  	4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6, 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12,\
  	5, 6, 3, 0, 14, 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3};
	(*des_base).s_6 = {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, 10, 15, 4, 2,\
  	7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8, 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10,\
  	1, 13, 11, 6, 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13};
	(*des_base).s_7 = {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1, 13, 0, 11, 7,\
  	4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6, 1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6,\
  	8, 0, 5, 9, 2, 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12};
	(*des_base).s_8 = {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, 1, 15, 13, 8,\
  	10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2, 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10,\
  	13, 15, 3, 5, 8, 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11};
	(*des_base).shift_32bits = { 6, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31,\
  	10, 2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25};
	(*des_base).p_shift = {16,	7, 20, 21, 29, 12, 28, 17, 1, 15,	23,	26,	5, 18, 31, 10,\
  	2, 8, 24, 14, 32,	27,	3, 9, 19,	13,	30,	6, 22, 11, 4,	25};

}*/

/*main des-encryption function */
void des_enc(t_args *params)
{
  int i;
  int j;
  int k;
	int m;
  unsigned char left[4];
  unsigned char right[4];
	unsigned char right_f[4];
  unsigned char right48[6];
	unsigned char exp_for_s[8];
  int str_col[8][2];
  int s_output;
  int *four_bits;
	int rounds;
	m = 0;
	four_bits = NULL;
  printf("1BUF%s\n", (*params).buf);
  const int r_to_48[48] = {32,	1, 2,	3, 4,	5, 4,	5, 6,	7, 8,	9, 8,	9, 10, 11, 12, 13,\
  12,	13,	14,	15,	16,	17, 16,	17,	18,	19,	20,	21, 20,	21,	22,	23,	24,	25, 24,\
  25,	26,	27,	28,	29, 28,	29,	30,	31,	32,	1};

  /*const int s_1[4][16] = {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
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
  {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}};*/

	const int s_1[8][4][16] = {{{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
    {0, 15, 7, 4,	14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
    {4, 1, 14, 8, 13, 6, 2, 11, 15, 12,	9, 7, 3, 10, 5, 0},
    {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
	{{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
  {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
  {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
  {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
	{{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
  {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
  {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
  {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
	{{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
  {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
  {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
  {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
	{{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
  {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
  {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
  {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
	{{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
  {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
  {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
  {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
	{{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
  {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
  {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
  {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
	{{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
  {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
  {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
  {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

	const int p_shift[32] = {16,	7, 20, 21, 29, 12, 28, 17, 1, 15,	23,	26,	5, 18, 31, 10,\
  	2, 8, 24, 14, 32,	27,	3, 9, 19,	13,	30,	6, 22, 11, 4,	25};
  //t_des_tables des_base;

  //fill_des_tables(&des_base);
  i = 0;
  j = 0;
  k = 0;
	rounds = 0;
  s_output = 0;
  message_first_shift(params);
  printf("M after first shift%s\n", (*params).buf);
  while (i < 4)
  {
    left[i] = (*params).buf[j];
    i++;
    j++;
  }
  i = 0;
  while (i < 4)
  {
    left[i] = (*params).buf[j];
    i++;
    j++;
  }
  i = 0;
  j = 0;
  while (i < 6)
  {
    right48[i] = 0;
    i++;
  }
	while (rounds < 16)
  {
		i = 0;
  while (i < 6)
  {
    j = 7;
    while (j >= 0)
    {
      right48[i] |= ((1 << ((r_to_48[k] % 8) - 1)) &
        right[(r_to_48[k] - (r_to_48[k] % 8)) / 8]) << j;
      k++;
      j--;
    }
    i++;
  }
  printf("Right to 48%s\n", right48);
  make_keys(params, rounds);
  i = 0;
  while (i < 6)
  {
    right48[i] ^= (*params).key_res[i];
    i++;
  }
  printf("Right after XOR%s\n", right48);
  //string[0] = right48[0] >> 7;
  //string[1] = ((right48[0] << 5) + (right48[0] >> 2));
	exp_for_s[0] = (right48[0] >> 2) & 255;
	exp_for_s[1] = ((right48[0] << 6) & 255) + (right48[1] >> 4);
	exp_for_s[2] = ((right48[1] << 4) & 255) + (right48[2] >> 6);
	exp_for_s[3] = (right48[2] << 2) & 255;
	exp_for_s[4] = (right48[3] >> 2) & 255;
	exp_for_s[5] = ((right48[3] << 6) & 255) + (right48[4] >> 4);
	exp_for_s[6] = ((right48[4] << 4) & 255) + (right48[5] >> 6);
	exp_for_s[7] = (right48[5] << 2) & 255;
	printf("right made to 8 bits%s\n", exp_for_s);
  //column[0] = (right48[0] << 1) + (right48[0] >> 6);
  //column[1] = (right48[0] << 2) + (right48[0] >> 5);
  //column[2] = (right48[0] << 3) + (right48[0] >> 4);
  //column[3] = (right48[0] << 4) + (right48[0] >> 3);
	k = 0;
	while (k < 8)
	{
		str_col[k][0] = ((exp_for_s[k] >> 7) - '0') * 2 + (((exp_for_s[k] << 5) + (exp_for_s[k] >> 2)) - '0');
		str_col[k][1] = (((exp_for_s[k] << 1) + (exp_for_s[k] >> 6)) - '0') * 8 + (((exp_for_s[k] << 2)
		 + (exp_for_s[k] >> 5)) - '0') * 4 + (((exp_for_s[k] << 3) + (exp_for_s[k] >> 4)) - '0')
		  * 2 + (((exp_for_s[k] << 4) + (exp_for_s[k] >> 3)) - '0');
		printf("%d\n", str_col[k][0]);
		/*i = 0;
		j = 6;
		while (i < 4)
		{
			if (four_bits[i])
		    exp_for_s[0] |= (1 << j);
		  else
		    exp_for_s[0] &= ~(1 << j);
			i++;
			j--;
		}*/
		k++;
	}
	i = 0;
	k = 0;
  while (i < 4)
  {
    j = 7;
    while (j >= 0)
    {
			s_output = s_1[m++][str_col[0][0]][str_col[0][1]];
			printf("%d\n", s_output);
			to_binary(&four_bits, s_output, 2);
			//s_output1 = s_1[str_col[0]][str_col[1]];
			k = 0;
			while (k < 4)
			{
				if (four_bits[k])
			    right[i] |= (1 << j);
			  else
			    right[i] &= ~(1 << j);
				k++;
				j--;
			}
			/*s_output = s_1[m++][str_col[0][0]][str_col[0][1]];
			to_binary(&four_bits, s_output, 2);
			//s_output1 = s_1[str_col[0]][str_col[1]];
			k = 0;
			while (k < 4)
			{
				if (four_bits[k])
			    right[i] |= (1 << j);
			  else
			    right[i] &= ~(1 << j);
				k++;
				j--;
			}*/
    }
    i++;
  }
		printf("right%s\n", right);
	/*right48[0] = ((exp_for_s[0] >> 2) & 255) + (exp_for_s[1] >> 6);
	right48[1] = ((exp_for_s[1] << 2) & 255) + (exp_for_s[1] >> 2) + (exp_for_s[2] >> 4);
	right48[2] = ((exp_for_s[2] << 4) & 255) + (exp_for_s[2] >> 2) + (exp_for_s[3] >> 2);
	right48[3] = ((exp_for_s[4] >> 2) & 255) + (exp_for_s[5] >> 6);
	right48[4] = ((exp_for_s[5] << 2) & 255) + (exp_for_s[5] >> 2) + (exp_for_s[6] >> 4);
	right48[5] = ((exp_for_s[6] << 4) & 255) + (exp_for_s[5] >> 2) + (exp_for_s[7] >> 2);*/
	i = 0;
  while (i < 4)
  {
    j = 7;
    while (j >= 0)
    {
      right_f[i] |= ((1 << ((p_shift[k] % 8) - 1)) &
        right[(p_shift[k] - (p_shift[k] % 8)) / 8]) << j;
      k++;
      j--;
    }
    i++;
  }
	printf("right_f%s\n", right_f);
	i = 0;
	while (i < 4)
	{
		right[i] = left[i];
		i++;
	}
	i = 0;
	while (i < 4)
  {
    left[i] ^= right_f[i];
    i++;
  }
	rounds++;
}
	printf("new right%s\n", right);
	printf("new left%s\n", left);
i = 0;
while (i < 4)
{
	(*params).des_output[i] = left[i];
	i++;
}
i = 0;
while (i < 4)
{
	(*params).des_output[i] = right[i];
	i++;
}
printf("%s\n", (*params).des_output);
}

void des_read(t_args *params, char **argv)
{
  int i;
  int ret;

  ret = 0;
  if ((find_symb((*params).flags, 'i', FLAG_LEN)) >= 0)
  {
    while ((ret = read((*params).ifd, params->buf, DES_BLOCK)) > 0)
    {
      //ft_printf("BUF %s\n", (*params).buf);
      if ((find_symb((*params).flags, 'd', FLAG_LEN)) >= 0)
        des_dec(params);
      else
        des_enc(params);
      if (ret < DES_BLOCK)
        break;
      i = 0;
      while (i < DES_BLOCK)
      {
        (*params).buf[i] = 0;
        i++;
      }
    }
  }
  else if ((ret = read(0, params->buf, DES_BLOCK)) > 0)
  {
    ft_printf("BUF %s\n", (*params).buf);
    if ((find_symb((*params).flags, 'd', FLAG_LEN)) >= 0)
      des_dec(params);
    else
      des_enc(params);
    /*else if (ft_strcmp(argv[1], "des") == 0)
      des_enc(buf);
    else if (ft_strcmp(argv[1], "des-ecb") == 0)
      des_ecb_enc(buf);
    else if (ft_strcmp(argv[1], "des-cbc") == 0)
      des_cbc_enc(buf);*/
    i = 0;
    while (i < DES_BLOCK)
    {
      (*params).buf[i] = 0;
      i++;
    }
    while ((ret = read(0, params->buf, DES_BLOCK)) > 0)
    {
      //ft_printf("BUF %s\n", (*params).buf);
      if ((find_symb((*params).flags, 'd', FLAG_LEN)) >= 0)
        des_dec(params);
      else
        des_enc(params);
      if (ret < DES_BLOCK)
        break;
      i = 0;
      while (i < DES_BLOCK)
      {
        (*params).buf[i] = 0;
        i++;
      }
    }
  }
  else if ((ret = read(1, params->buf, DES_BLOCK)) > 0)
  {
      //ft_printf("%s\n", (*params).buf);
      if ((find_symb((*params).flags, 'd', FLAG_LEN)) >= 0)
        des_dec(params);
      else
        des_enc(params);
      i = 0;
      while (i < DES_BLOCK)
      {
        (*params).buf[i] = 0;
        i++;
      }
      while ((ret = read(1, params->buf, DES_BLOCK)) > 0)
      {
        //ft_printf("%s\n", (*params).buf);
        if ((find_symb((*params).flags, 'd', FLAG_LEN)) >= 0)
          des_dec(params);
        else
          des_enc(params);
        if (ret < DES_BLOCK)
          break;
        i = 0;
        while (i < DES_BLOCK)
        {
          (*params).buf[i] = 0;
          i++;
        }
      }
  }
  if ((find_symb((*params).flags, 'o', FLAG_LEN)) < 0)
    ft_printf("%s", "\n");
}

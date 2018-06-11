/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_blocks_proc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/10 17:03:54 by vlikhotk          #+#    #+#             */
/*   Updated: 2018/06/10 17:05:39 by vlikhotk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int		s1_block(int str, int col)
{
	const int s_1[4][16] = {{14, 4, 13, 1, 2, 15, 11, 8, 3,
	10, 6, 12, 5, 9, 0, 7}, {0, 15, 7, 4, 14, 2, 13, 1, 10,
	6, 12, 11, 9, 5, 3, 8}, {4, 1, 14, 8, 13, 6, 2, 11, 15,
	12, 9, 7, 3, 10, 5, 0}, {15, 12, 8, 2, 4, 9, 1, 7, 5,
	11, 3, 14, 10, 0, 6, 13}};

	return (s_1[str][col]);
}

int		s2_block(int str, int col)
{
	const int s_2[4][16] = {{15, 1, 8, 14, 6, 11, 3, 4, 9,
	7, 2, 13, 12, 0, 5, 10}, {3, 13, 4, 7, 15, 2, 8, 14, 12,
	0, 1, 10, 6, 9, 11, 5}, {0, 14, 7, 11, 10, 4, 13, 1, 5,
	8, 12, 6, 9, 3, 2, 15}, {13, 8, 10, 1, 3, 15, 4, 2, 11,
	6, 7, 12, 0, 5, 14, 9}};

	return (s_2[str][col]);
}

int		s3_block(int str, int col)
{
	const int s_3[4][16] = {{10, 0, 9, 14, 6, 3, 15, 5, 1,
	13, 12, 7, 11, 4, 2, 8}, {13, 7, 0, 9, 3, 4, 6, 10, 2,
	8, 5, 14, 12, 11, 15, 1}, {13, 6, 4, 9, 8, 15, 3, 0,
	11, 1, 2, 12, 5, 10, 14, 7}, {1, 10, 13, 0, 6, 9, 8,
	7, 4, 15, 14, 3, 11, 5, 2, 12}};

	return (s_3[str][col]);
}

void	re_s_blocks_proc(t_des_enc *des_params,
int *s_output, t_addition *iters, int **four_bits)
{
	if ((*iters).i == 1)
		*s_output = s2_block((*des_params).str_col[(*iters).i][0],
		(*des_params).str_col[(*iters).i][1]);
	else if ((*iters).i == 3)
		*s_output = s4_block((*des_params).str_col[(*iters).i][0],
		(*des_params).str_col[(*iters).i][1]);
	else if ((*iters).i == 5)
		*s_output = s6_block((*des_params).str_col[(*iters).i][0],
		(*des_params).str_col[(*iters).i][1]);
	else if ((*iters).i == 7)
		*s_output = s8_block((*des_params).str_col[(*iters).i][0],
		(*des_params).str_col[(*iters).i][1]);
	(*iters).i++;
	to_binary(four_bits, *s_output, 2);
	(*iters).k = 0;
	(*iters).j = 3;
	while ((*iters).k < 4)
	{
		if ((*four_bits)[(*iters).k])
			(*des_params).right[(*iters).m] |= (1 << (*iters).j);
		else
			(*des_params).right[(*iters).m] &= ~(1 << (*iters).j);
		(*iters).k++;
		(*iters).j--;
	}
}

/*
** Step 6.2 Receive string and column numbers
*/

void	s_blocks_coords(t_des_enc *des_params, unsigned char exp_for_s[])
{
	int j;
	int string[2];
	int column[4];

	j = 0;
	while (j < 8)
	{
		string[0] = ((1 << 5) & exp_for_s[j]) ? 1 : 0;
		string[1] = ((1 << 0) & exp_for_s[j]) ? 1 : 0;
		column[0] = ((1 << 4) & exp_for_s[j]) ? 1 : 0;
		column[1] = ((1 << 3) & exp_for_s[j]) ? 1 : 0;
		column[2] = ((1 << 2) & exp_for_s[j]) ? 1 : 0;
		column[3] = ((1 << 1) & exp_for_s[j]) ? 1 : 0;
		(*des_params).str_col[j][0] = string[0] * 2 + string[1];
		(*des_params).str_col[j][1] = column[0] * 8 + column[1] * 4
		+ column[2] * 2 + column[3];
		j++;
	}
}

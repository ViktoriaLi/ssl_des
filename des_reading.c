#include "ft_ssl.h"

void if_decr_block(t_args *params, int len, int fd, int ret)
{
	int i;

	i = 0;
	if (len == 64)
	{
			ignore_newline(params, fd, ret, 0);
	if ((*params).tmpad[0] != 0)
	{
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
}

void a_decr(t_args *params, int *ret)
{
	t_addition iters;

	clear_iterators(&iters);
	if ((find_symb((*params).flags, 'a', FLAG_LEN)) >= 0 &&
	find_symb((*params).flags, 'd', FLAG_LEN) >= 0)
	{
			(*params).desad_count = 0;
			make_short_blocks(params, *ret, 4, (*params).des_48_read);
			iters.i = 0;
			while (iters.i < (*params).desad_count)
			{
				(*params).des_48_read[iters.i] = (*params).des_48_res[iters.i];
				iters.i++;
			}
			*ret = (*params).desad_count - (*params).desad_count % 8;
		}
}

void a_encr(t_args *params, int ret, t_addition *iters)
{
	static unsigned char tmpb64[4];

	if ((find_symb((*params).flags, 'a', FLAG_LEN)) >= 0 &&
	find_symb((*params).flags, 'd', FLAG_LEN) < 0)
	{
			(*iters).i = 0;
			while ((*iters).i < (*iters).m)
			{
				(*iters).j = 0;
				while ((*iters).i < (*iters).m && (*iters).j < 3)
					tmpb64[(*iters).j++] = (*params).des_48_res[(*iters).i++];
				base64_enc(tmpb64, params, (*iters).j);
				(*iters).j = 0;
				while ((*iters).j < 3)
					tmpb64[(*iters).j++] = 0;
			}
			if (ret == 64 && (find_symb((*params).flags, 'o', FLAG_LEN)) >= 0)
				 write((*params).ofd, "\n", 1);
			else if (ret == 48)
				 write(1, "\n", 1);
		(*iters).i = 0;
		while ((*iters).i < (*iters).m)
			(*params).des_48_res[(*iters).i++] = 0;
		(*iters).m = 0;
	}
}

void prepare_for_encr(t_args *params, t_addition *iters, int *count, int ret)
{
	(*iters).i = 0;
	while ((*iters).i < ret)
	{
		(*iters).j = 0;
		while ((*iters).i < ret && (*iters).j < 8)
			(*params).buf[(*iters).j++] = (*params).des_48_read[(*iters).i++];
		(*count)++;
		des_enc(params, *count, &iters->m);
	}
}

void des_reading(int fd, t_args *params, int len)
{
	t_addition iters;
	int count;
  int ret;

	ret = 0;
	count = 0;
	clear_iterators(&iters);
	while ((ret = read(fd, params->des_48_read, len)) > 0)
	{
		if_decr_block(params, len, fd, ret);
		if (ret == len && (find_symb((*params).flags, 'd', FLAG_LEN)) < 0)
			(*params).if_full = 1;
		add_padding(params, &ret, len);
		a_decr(params, &ret);
		prepare_for_encr(params, &iters, &count, ret);
		a_encr(params, ret, &iters);
		clear_iterators(&iters);
		while (iters.i < ret)
			(*params).des_48_read[iters.i++] = 0;
		}
		if ((*params).if_full == 1 && (find_symb((*params).flags, 'd', FLAG_LEN)) < 0)
		{
			count++;
			while (iters.j < 8)
				(*params).buf[iters.j++] = 8;
			des_enc(params, count, &iters.m);
		}
}

#include "ft_ssl.h"

void	add_padding(t_args *params, int *ret, int len)
{
	t_addition iters;

	clear_iterators(&iters);
	if ((find_symb((*params).flags, 'd', FLAG_LEN)) < 0 &&
		(*ret % 8) == 0 && *ret != len)
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

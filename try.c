#include <stdio.h>
#include <stdlib.h>

size_t		ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (s[i] != 0)
		i++;
	return (i);
}

void	print_hex(char **res, int nbr, unsigned int base)
{
	int			i;
	int			len;
	int	d;

	d = nbr;
	len = 1;
	i = 0;
	while (d > base - 1)
	{
		d = d / base;
		len++;
	}
	if (!((*res) = (char *)malloc((len + 1))))
		return ;
	(*res)[len--] = 0;
	while (len >= 0)
	{
		if ((nbr % base) < 10)
			(*res)[len--] = (nbr % base) + 48;
		nbr = nbr / base;
	}
}

/*void	to_binary(int **res, int nbr, unsigned int base)
{
	int i;
	int			len;
	int	d;
  int *tmp;

  d = nbr;
	len = 0;
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
	while (len >= 0)
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
		d--;
    }
  }
}*/

/*
void	to_binary(int **res, int nbr, unsigned int base)
{
	int i;
	int	len;
	int	d;
  int *tmp;

  d = nbr;
	len = 0;
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
	while (len >= 0)
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
      (*res)[d--] = tmp[len--];
    while (d >= 0)
  		(*res)[d--] = 0;
  }
}
*/

int main()
{
  int *res;
  res = NULL;

	int test = 255;
	//printf("%d\n", test >> 1);
	//printf("%d\n", 14 / 8);
  to_binary(&res, 5, 2);
  printf("%d %d %d %d\n", res[0], res[1], res[2], res[3]);
	to_binary(&res, 12, 2);
  printf("%d %d %d %d\n", res[0], res[1], res[2], res[3]);
	to_binary(&res, 8, 2);
  printf("%d %d %d %d\n", res[0], res[1], res[2], res[3]);
	to_binary(&res, 2, 2);
  printf("%d %d %d %d\n", res[0], res[1], res[2], res[3]);
	to_binary(&res, 11, 2);
  printf("%d %d %d %d\n", res[0], res[1], res[2], res[3]);
	to_binary(&res, 5, 2);
  printf("%d %d %d %d\n", res[0], res[1], res[2], res[3]);
	to_binary(&res, 9, 2);
  printf("%d %d %d %d\n", res[0], res[1], res[2], res[3]);
	to_binary(&res, 7, 2);
  printf("%d %d %d %d\n", res[0], res[1], res[2], res[3]);
  /*unsigned char byte = 254;
  unsigned char res = 0;
  if ((1 << 0)  &  res)
    printf("%s\n", "yes");
  else
    printf("%s\n", "no");
  printf("%d\n", res);
  res |= ((1 << 0)  &  byte) << 0;
  if ((1 << 0)  &  res)
    printf("%s\n", "yes");
  else
    printf("%s\n", "no");
  printf("%d\n", res);*/

}

if ((find_symb((*params).flags, 'i', FLAG_LEN)) >= 0)
{
	while ((ret = read((*params).ifd, params->des_48_read, len)) > 0)
	{
		if (ret == len && (find_symb((*params).flags, 'd', FLAG_LEN)) < 0)
			(*params).if_full = 1;
		//ft_printf("BUF %s\n", (*params).buf);
		/*if ((*params).des_48_read[ret - 1] == 10)
		{
			(*params).des_48_read[ret - 1] = 0;
			ret -= 1;
		}*/

		i = 0;
		while (i < ret)
		{
			j = 0;
			while (i < ret && j < 8)
				(*params).buf[j++] = (*params).des_48_read[i++];
			count++;
			if ((find_symb((*params).flags, 'd', FLAG_LEN)) < 0)
			{
				if (j < 8)
				{
					k = 8 - j;
					while (j < 8)
						(*params).buf[j++] = k;
				}
			}
			//printf("TEST %d %d %d %d %d %d %d %d\n", (*params).buf[0], (*params).buf[1], (*params).buf[2],
		//(*params).buf[3], (*params).buf[4], (*params).buf[5], (*params).buf[6], (*params).buf[7]);
			des_enc(params, count, &l);
			}
			if ((find_symb((*params).flags, 'd', FLAG_LEN)) < 0 && (ret % 8) == 0 && ret != len)
			{
				j = 0;
				while (j < 8)
					(*params).buf[j++] = 8;
				des_enc(params, count, &l);
			}
			if ((find_symb((*params).flags, 'a', FLAG_LEN)) >= 0)
			{
				if (find_symb((*params).flags, 'd', FLAG_LEN) >= 0)
				{
					if ((*params).des_48_res[l - 1] > 0 && (*params).des_48_res[l - 1] < 9)
					{
						j = (*params).des_48_res[7] - 48;
						i = l - 1;
						while (j > 0)
						{
							(*params).des_48_res[i--] = 0;
							j--;
						}
					}
					i = 0;
					while (i < l)
					{
						j = 0;
						while (i < l && j < 4)
							tmpb64[j++] = (*params).des_48_res[i++];
						//ft_printf("%s\n", tmpb64d);
						base64_dec(tmpb64, params);
						j = 0;
						while (j < 4)
							tmpb64[j++] = 0;
					}
				}
				else
				{
					i = 0;
					while (i < l)
					{
						j = 0;
						while (i < l && j < 3)
							tmpb64[j++] = (*params).des_48_res[i++];
						//printf("CCC%d %d %d %d \n", tmpb64[0], tmpb64[1], tmpb64[2], tmpb64[3]);
						base64_enc(tmpb64, params, j);
						j = 0;
						while (j < 3)
							tmpb64[j++] = 0;
					}
				}
				i = 0;
				while (i < l)
					(*params).des_48_res[i++] = 0;
				l = 0;
			}
	}
	if ((*params).if_full == 1 && (find_symb((*params).flags, 'd', FLAG_LEN)) < 0)
	{
		j = 0;
		while (j < 8)
			(*params).buf[j++] = 8;
		des_enc(params, count, &l);
	}
}

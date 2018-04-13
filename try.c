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

int main()
{
  int *res;
  res = NULL;
	int test = 255;
	printf("%d\n", test >> 1);
	printf("%d\n", 14 / 8);
  //to_binary(&res, 6, 2);

  //printf("%d %d %d %d\n", res[0], res[1], res[2], res[3]);
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
  /*while (i < DES_BLOCK)
    buf_res[i++] = 0;
  i = 0;*/
  while (i < DES_BLOCK)
  {
    j = 7;
    while (j >= 0)
    {
      //printf("d%d\n", ((m_start[k] % 8) - 1));
      //printf("c%d\n", (*params).buf[(m_start[k] - (m_start[k] % 8)) / 8]);
			if ((m_start[k] % 8) > 0)
			{
				if (((1 << (8 - (m_start[k] % 8))) &
	      (*params).buf[m_start[k] / 8]))
	        buf_res[i] |= (1 << j);
	      else
	        buf_res[i] &= ~(1 << j);
	      k++;
	      j--;
			}
			else
			{
				if (((1 << 0) & (*params).buf[m_start[k] / 8]))
	        buf_res[i] |= (1 << j);
	      else
	        buf_res[i] &= ~(1 << j);
	      k++;
	      j--;
			}
    }
    i++;
  }
	//printf("CODE FSHIFT%d %d %d %d %d %d %d %d\n", buf_res[0], buf_res[1], buf_res[2], buf_res[3],
	//buf_res[4], buf_res[5], buf_res[6], buf_res[7]);
  i = 0;
  printf("first shift%s\n", buf_res);
  while (i < 8)
  {
    (*params).buf[i] = buf_res[i];
    i++;
  }
}

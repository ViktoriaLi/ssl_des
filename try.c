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

  to_binary(&res, 6, 2);

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

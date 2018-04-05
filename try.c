#include <stdio.h>
#include <stdlib.h>

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

int main()
{
  char *res;
  res = NULL;

  print_hex(&res, 6, 2);

  printf("%s\n", res);
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

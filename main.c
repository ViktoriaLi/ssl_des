/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/21 13:47:06 by vlikhotk          #+#    #+#             */
/*   Updated: 2018/03/21 13:47:10 by vlikhotk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <stdio.h>

int main (int argc, char **argv)
{
  int i;
  int j;
  int fd;
  int ret;
  char *base64_str;
  char *buf;
  char dest[5];
  //int *tmp;
  int dest_len;

  i = 0;
  j = 0;
  //buf = NULL;
  //if (argc == 1)
    //ft_printf("%s\n", "usage: ft_ssl command [command opts] [command args]");
  dest[4] = 0;
  base64_str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  fd = open(argv[1], O_RDONLY);
  printf("%d\n", fd);
  //tmp = (int *)malloc(sizeof(int) * (dest_len + 1));
  while (get_next_line(1, &buf) == 1)
  {
  //  dest_len = ft_strlen(buf) * 8 / 6;

    printf("%s\n", buf);

  //dest = (char *)malloc(dest_len + 1);
  //dest[dest_len] = 0;

  /*  while (j <= ft_strlen(buf))
    {
      dest[0] = 0;
      dest[1] = 0;
      dest[2] = '=';
      dest[3] = '=';
      dest[0] = (unsigned char)base64_str[buf[j] >> 2];
      dest[1] = (unsigned char)base64_str[((buf[j] & 3) << 4) + ((buf[j + 1] & 240) >> 4)];

      //dest[1] = base64_str[((buf[j] & 3) << 4)];
    if (buf[j + 1])
    {
      dest[2] = (unsigned char)base64_str[((buf[j + 1] & 15) << 2) + ((buf[j + 2] & 192) >> 6)];

    }
    if (buf[j + 2])
      {
        dest[3] = (unsigned char)base64_str[buf[j + 2] & 63];
        //dest[3] = '=';
      }*/
    /*  dest[0] = base64_str[buf[j] >> 2];
      dest[1] = (unsigned char)base64_str[((buf[j] & 3) << 4) | (buf[j + 1] >> 4)];
       if (buf[j + 1])
       {
         dest[2] = (unsigned char)base64_str[((buf[j + 1] & 15) << 2) | (buf[j + 2] >> 6)];
       }

      if (buf[j + 2])
        dest[3] = (unsigned char)base64_str[buf[j + 2] & 63];*/

    /*if (!buf[j + 1])
    {
      dest[2] = '=';
      dest[3] = '=';
    }
    if (!buf[j + 2])
      dest[3] = '=';*/
      /*while (i < dest_len)
      {
        tmp[i] = buf >> (dest_len - i ) * 6 + buf << i;
        i++;
      }
      i = 0;
      while (i < dest_len)
      {
        dest[i] = base64_str[tmp[i]];
        i++;
      }*/
    //j += 3;
    //printf("%s\n", dest);
  //}
}
}

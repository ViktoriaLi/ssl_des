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

void base64_enc(char *buf)
{
  char dest[5];

  dest[2] = '=';
  dest[3] = '=';
  dest[4] = 0;
  dest[0] = BASE64_STR[buf[0] >> 2];
  dest[1] = BASE64_STR[((buf[0] & 3) << 4) + ((buf[1]) >> 4)];
  if (ft_strlen(buf) > 1)
    dest[2] = BASE64_STR[((buf[1] & 15) << 2) + ((buf[2]) >> 6)];
  if (ft_strlen(buf) > 2)
    dest[3] = BASE64_STR[buf[2] & 63];
  ft_printf("%s", dest);
}

int main (int argc, char **argv)
{
  int fd;
  int ret;
  char buf[4];
  int i;
  //t_argc params;

  fd = 0;
  buf[3] = 0;
  i = 0;
  if (argc == 1)
  {
    ft_printf("%s\n", "usage: ft_ssl command [command opts] [command args]");
    return (0);
  }
  if ((ret = read(0, &buf, 3)) > 0)
  {
    //ft_printf("%s\n", buf);
    if (ft_strcmp(argv[1], "base64") == 0)
      base64_enc(buf);
    i = 0;
    while (i < 3)
    {
      buf[i] = 0;
      i++;
    }
    while ((ret = read(0, &buf, 3)) > 0)
    {
      //ft_printf("%s\n", buf);
      if (ft_strcmp(argv[1], "base64") == 0)
        base64_enc(buf);
      i = 0;
      while (i < 3)
      {
        buf[i] = 0;
        i++;
      }
    }
  }
  else if (ft_strcmp(argv[2], "-i") == 0)
  {
    fd = open(argv[3], O_RDONLY);
    while ((ret = read(fd, &buf, 3)) > 0)
    {
      //ft_printf("%s\n", buf);
      if (ft_strcmp(argv[1], "base64") == 0)
        base64_enc(buf);
      i = 0;
      while (i < 3)
      {
        buf[i] = 0;
        i++;
      }
    }
  }
  else if (ft_strcmp(argv[3], "-i") == 0)
  {
    fd = open(argv[4], O_RDONLY);
    while ((ret = read(fd, &buf, 3)) > 0)
    {
      //ft_printf("%s\n", buf);
      if (ft_strcmp(argv[1], "base64") == 0 && ft_strcmp(argv[2], "-e") == 0)
        base64_enc(buf);
      i = 0;
      while (i < 3)
      {
        buf[i] = 0;
        i++;
      }
    }
  }
  else if ((ret = read(1, &buf, 3)) > 0)
  {
    i = 0;
    while (i < 3)
    {
      buf[i] = 0;
      i++;
    }
      //ft_printf("%s\n", buf);
      if (ft_strcmp(argv[1], "base64") == 0)
        base64_enc(buf);
      while ((ret = read(1, &buf, 3)) > 0)
      {
        //ft_printf("%s\n", buf);
        if (ft_strcmp(argv[1], "base64") == 0)
          base64_enc(buf);
        i = 0;
        while (i < 3)
        {
          buf[i] = 0;
          i++;
        }
      }
  }
  ft_printf("%s", "\n");
}

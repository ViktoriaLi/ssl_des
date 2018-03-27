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

void base64_dec(char *buf)
{

}

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

void base64_dread(t_argc *params, char **argv)
{

}

void base64_eread(t_argc *params, char **argv)
{
  int i;
  int ret;
  int fd;
  char buf[4];

  fd = 0;
  buf[3] = 0;
  ret = 0;
  if (find_flag(params, 'i'))
  {
    fd = open(argv[(*params).ifile], O_RDONLY);
    while ((ret = read(fd, &buf, 3)) > 0)
    {
      //ft_printf("%s\n", buf);
      base64_enc(buf);
      i = 0;
      while (i < 3)
      {
        buf[i] = 0;
        i++;
      }
    }
  }
  else if ((ret = read(0, &buf, 3)) > 0)
  {
    //ft_printf("%s\n", buf);
    base64_enc(buf);
    /*else if (ft_strcmp(argv[1], "des") == 0)
      des_enc(buf);
    else if (ft_strcmp(argv[1], "des-ecb") == 0)
      des_ecb_enc(buf);
    else if (ft_strcmp(argv[1], "des-cbc") == 0)
      des_cbc_enc(buf);*/
    i = 0;
    while (i < 3)
    {
      buf[i] = 0;
      i++;
    }
    while ((ret = read(0, &buf, 3)) > 0)
    {
      //ft_printf("%s\n", buf);
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
      base64_enc(buf);
      while ((ret = read(1, &buf, 3)) > 0)
      {
        //ft_printf("%s\n", buf);
        base64_enc(buf);
        i = 0;
        while (i < 3)
        {
          buf[i] = 0;
          i++;
        }
      }
  }
}

int find_flag(t_argc *params, char flag)
{
  int i;

  i = 0;
  while (i < 5)
  {
    if ((*params).flags[i] == flag)
      return (1);
    i++;
  }
  return (0);
}

void flags_normalize(char *all_flags, t_argc *params, int len)
{
  int i;
  int j;

  i = 0;
  j = 0;
  while (i < len)
  {
    j = 0;
    while ((*params).flags[j] && (*params).flags[j] != all_flags[i])
      j++;
    if ((*params).flags[j] != all_flags[i])
      (*params).flags[j] = all_flags[i];
    i++;
  }
}

int check_ssl_flags(int argc, char **argv, t_argc *params)
{
  int i;
  int j;
  char *all_flags;

  j = 0;
  i = 2;
  all_flags = (char *)malloc(argc - 1);
  all_flags[argc - 1] = 0;
  while (i < argc)
  {
    if (ft_strcmp(argv[i], "-e") == 0 || ft_strcmp(argv[i], "-d") == 0 ||
    ft_strcmp(argv[i], "-i") == 0 || ft_strcmp(argv[i], "-o") == 0 || ft_strcmp(argv[i], "-D") == 0)
    {
      if (argv[i][1] == 'i')
        (*params).ifile = i;
      if (argv[i][1] == 'o')
        (*params).ofile = i;
      all_flags[j] = argv[i][1];
      j++;
      i++;
    }
    else
      return (i);
  }
  flags_normalize(all_flags, params, argc - 1);
  if (find_flag(params, 'i') && (*params).ifile == 0)
  {
    ft_printf("base64: option requires an argument -- i\n");
    return (1);
  }
  if (find_flag(params, 'o') && (*params).ofile == 0)
  {
    ft_printf("base64: option requires an argument -- o\n");
    return (1);
  }
  return (0);
}

int if_valid_args(int argc, char **argv, t_argc *params)
{
  int res;

  res = 0;
  if (argc == 1)
  {
    ft_printf("%s\n", "usage: ft_ssl command [command opts] [command args]");
    return (0);
  }
  if (ft_strcmp(argv[1], "base64") != 0 && ft_strcmp(argv[1], "des") != 0 &&
  ft_strcmp(argv[1], "des-ecb") != 0 && ft_strcmp(argv[1], "des-cbc") != 0)
  {
    ft_printf("ft_ssl: Error: %s is an invalid command.\n\n", argv[1]);
    ft_printf("%s\n", "Standard commands:\n\nMessage Digest commands:\n\nCipher commands:");
    ft_printf("%s\n", "base64\ndes\ndes-ecb\ndes-cbc");
    return (0);
  }
  res = check_ssl_flags(argc, argv, params);
  if (res > 0)
  {
    if (res > 1)
      ft_printf("base64: invalid option -- %c\n", argv[res][1]);
    return (0);
  }
  return (1);
}

int main (int argc, char **argv)
{
  t_argc params;

  if (!if_valid_args(argc, argv, &params))
    return (0);
  if (ft_strcmp(argv[1], "base64") == 0 && !find_flag(&params, 'd'))
    base64_eread(&params, argv);
  else if (ft_strcmp(argv[1], "base64") == 0 && find_flag(&params, 'd'))
    base64_dread(&params, argv);
  ft_printf("%s", "\n");
}

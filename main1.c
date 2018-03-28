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

int find_symb(char *str, char flag, int len)
{
  int i;

  i = 0;
  while (i < len)
  {
    if (str[i] == flag)
      return (i);
    i++;
  }
  return (-1);
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
      all_flags[j] = argv[i][1];
      if (argv[i][1] == 'i')
      {
        (*params).ifd = open(argv[i + 1], O_RDONLY);
        i++;
      }
      else if (argv[i][1] == 'o')
      {
        (*params).ofd = open(argv[i + 1], O_RDWR | O_APPEND);;
        i++;
      }
      j++;
      i++;
    }
    else
    {
      ft_printf("base64: invalid option -- %c\n", argv[i][1]);
      return (1);
    }
  }
  flags_normalize(all_flags, params, argc - 1);
  //printf("DDD%d\n", find_flag(params, 'i'));
  //printf("DDD%s\n", (*params).flags);
  //printf("DDD%d\n", (*params).ifd);
  if ((find_symb((*params).flags, 'i', 5)) >= 0 && (*params).ifd < 0)
  {
    ft_printf("%s\n", "base64: option requires an argument -- i");
    return (1);
  }
  if ((find_symb((*params).flags, 'o', 5)) >= 0 && (*params).ofd < 0)
  {
    ft_printf("%s\n", "base64: option requires an argument -- o");
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
  if ((res = check_ssl_flags(argc, argv, params)) > 0)
    return (0);
  return (1);
}

void clear_struct(t_argc *params)
{
  int i;

  i = 0;
  (*params).cipher = NULL;
  (*params).buf = NULL;
  while (i < 6)
  {
    (*params).flags[i] = 0;
    i++;
  }
	(*params).ifd = 0;
	(*params).ofd = 0;
}

int main (int argc, char **argv)
{
  t_argc params;

  clear_struct(&params);
  if (!if_valid_args(argc, argv, &params))
    return (0);
  if (ft_strcmp(argv[1], "base64") == 0 && (find_symb(params.flags, 'd', 5)) < 0)
    base64_read(&params, argv, 3);
  else if (ft_strcmp(argv[1], "base64") == 0 && (find_symb(params.flags, 'd', 5)) >= 0)
    base64_read(&params, argv, 4);
  if ((*params).ifd > 1)
    close((*params).ifd);
  if ((*params).ofd > 1)
    close((*params).ofd);
}

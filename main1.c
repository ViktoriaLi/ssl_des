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

int check_b64_flags(int argc, char **argv, t_args *params)
{
  int i;
  int j;
  char *all_flags;

  j = 0;
  i = 2;
  if (!(all_flags = (char *)malloc(argc - 1)))
		return (1);
  all_flags[argc - 1] = 0;
  while (i < argc)
  {
    if (ft_strcmp(argv[i], "-e") == 0 || ft_strcmp(argv[i], "-d") == 0 ||
    ft_strcmp(argv[i], "-i") == 0 || ft_strcmp(argv[i], "-o") == 0)
    {
      all_flags[j] = argv[i][1];
      if (argv[i][1] == 'i')
      {
        (*params).ifd = open(argv[i + 1], O_RDONLY);
        i++;
      }
      else if (argv[i][1] == 'o')
      {
        (*params).ofd = open(argv[i + 1], O_RDWR | O_APPEND);
        if ((*params).ofd < 0)
          (*params).ofd = open(argv[i + 1], O_CREAT , O_APPEND | O_RDWR);
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
  if ((find_symb((*params).flags, 'i', FLAG_LEN)) >= 0 && (*params).ifd < 0)
  {
    ft_printf("%s\n", "base64: option requires an argument -- i");
    return (1);
  }
  if ((find_symb((*params).flags, 'o', FLAG_LEN)) >= 0 && (*params).ofd < 0)
  {
    ft_printf("%s\n", "base64: option requires an argument -- o");
    return (1);
  }
  return (0);
}

int check_des_flags(int argc, char **argv, t_args *params)
{
  int i;
  int j;
  char *all_flags;

  j = 0;
  i = 2;
  if (!(all_flags = (char *)malloc(argc - 1)))
		return (1);
  all_flags[argc - 1] = 0;
  while (i < argc)
  {
    if (ft_strcmp(argv[i], "-e") == 0 || ft_strcmp(argv[i], "-d") == 0 ||
    ft_strcmp(argv[i], "-i") == 0 || ft_strcmp(argv[i], "-o") == 0 || ft_strcmp(argv[i], "-a") == 0
    || ft_strcmp(argv[i], "-k") == 0 || ft_strcmp(argv[i], "-v") == 0 || ft_strcmp(argv[i], "-s") == 0
   || ft_strcmp(argv[i], "-p") == 0)
    {
      all_flags[j] = argv[i][1];
      if (argv[i][1] == 'i')
      {
        (*params).ifd = open(argv[i + 1], O_RDONLY);
        i++;
      }
      if (argv[i][1] == 'o')
      {
        (*params).ofd = open(argv[i + 1], O_RDWR | O_APPEND);
        if ((*params).ofd == -1)
          (*params).ofd = open(argv[i + 1], O_RDWR | O_CREAT | 0666);
        i++;
      }
      if (argv[i][1] == 'k')
      {
        (*params).des_key = (unsigned char *)argv[i + 1];
        i++;
      }
      if (argv[i][1] == 'v')
      {
        (*params).vector16 = (unsigned char *)argv[i + 1];
        i++;
      }
      j++;
      i++;
    }
    else
    {
      ft_printf("ft_ssl:Error: '%s' is an invalid command.\n", argv[i]);
      return (1);
    }
    //printf("%s\n", (*params).vector16);
  }
  flags_normalize(all_flags, params, argc - 1);
  //printf("DDD%d\n", find_flag(params, 'i'));
  //printf("DDD%s\n", (*params).flags);
  //printf("DDD%d\n", (*params).ifd);
  if (((find_symb((*params).flags, 'i', FLAG_LEN)) >= 0 && (*params).ifd == -1)
  || ((find_symb((*params).flags, 'o', FLAG_LEN)) >= 0 && (*params).ofd == -1))
  {
    ft_printf("usage: ft_ssl %s [-i file] [-v IV] [-k key] [-o file]\n", argv[1]);
    return (1);
  }
  return (0);
}

int if_valid_args_des(int argc, char **argv, t_args *params)
{
  int res;

  res = 0;
  if (argc == 1)
  {
    ft_printf("%s\n", "usage: ft_ssl command [command opts] [command args]");
    return (0);
  }
  if (ft_strcmp(argv[1], "base64") != 0 && ft_strcmp(argv[1], "des") != 0 &&
  ft_strcmp(argv[1], "des-ecb") != 0 && ft_strcmp(argv[1], "des-cbc") != 0
  && ft_strcmp(argv[1], "des3") != 0 && ft_strcmp(argv[1], "des3-ecb") != 0
  && ft_strcmp(argv[1], "des3-cbc") != 0)
  {
    ft_printf("ft_ssl: Error: %s is an invalid command.\n\n", argv[1]);
    ft_printf("%s\n", "Standard commands:\n\nMessage Digest commands:\n\nCipher commands:");
    ft_printf("%s\n", "base64\ndes\ndes-ecb\ndes-cbc\ndes3\ndes3-ecb\ndes3-cbc");
    return (0);
  }
  if ((ft_strcmp(argv[1], "base64") == 0) && (res = check_b64_flags(argc, argv, params)) > 0)
    return (0);
  else if (((ft_strcmp(argv[1], "des") == 0) || (ft_strcmp(argv[1], "des-ecb") == 0)
  || (ft_strcmp(argv[1], "des-cbc") == 0) || (ft_strcmp(argv[1], "des3") == 0)
|| (ft_strcmp(argv[1], "des3-ecb") == 0) || (ft_strcmp(argv[1], "des3-cbc") == 0))
&& (res = check_des_flags(argc, argv, params)) > 0)
    return (0);
  (*params).cipher = argv[1];
  return (1);
}

int main (int argc, char **argv)
{
  t_args params;

  clear_struct(&params);
  if (!if_valid_args_des(argc, argv, &params))
    return (0);
    if (ft_strcmp(params.cipher, "base64") == 0 && find_symb(params.flags, 'd', FLAG_LEN) < 0
  && (find_symb(params.flags, 'i', FLAG_LEN)) >= 0)
  base64_reading(params.ifd, &params, 48);
    else if (ft_strcmp(params.cipher, "base64") == 0 && find_symb(params.flags, 'd', FLAG_LEN) < 0
  && (find_symb(params.flags, 'i', FLAG_LEN)) < 0)
      base64_reading(0, &params, 48);
    else if (ft_strcmp(params.cipher, "base64") == 0 && find_symb(params.flags, 'd', FLAG_LEN) >= 0
  && (find_symb(params.flags, 'i', FLAG_LEN)) >= 0)
      base64_reading(params.ifd, &params, 64);
      else if (ft_strcmp(params.cipher, "base64") == 0 && find_symb(params.flags, 'd', FLAG_LEN) >= 0
    && (find_symb(params.flags, 'i', FLAG_LEN)) < 0)
      base64_reading(0, &params, 64);

  else if (((ft_strcmp(params.cipher, "des") == 0) || (ft_strcmp(params.cipher, "des-ecb") == 0)
|| (ft_strcmp(params.cipher, "des-cbc") == 0)) && (find_symb(params.flags, 'd', FLAG_LEN) < 0)
&& (find_symb(params.flags, 'i', FLAG_LEN)) < 0)
    des_reading(0, &params, 48);
    else if (((ft_strcmp(params.cipher, "des") == 0) || (ft_strcmp(params.cipher, "des-ecb") == 0)
  || (ft_strcmp(params.cipher, "des-cbc") == 0)) && (find_symb(params.flags, 'd', FLAG_LEN) < 0)
&& (find_symb(params.flags, 'i', FLAG_LEN)) >= 0)
    des_reading(params.ifd, &params, 48);
  else if (((ft_strcmp(params.cipher, "des") == 0) || (ft_strcmp(params.cipher, "des-ecb") == 0)
|| (ft_strcmp(params.cipher, "des-cbc") == 0)) && (find_symb(params.flags, 'd', FLAG_LEN) >= 0)
&& (find_symb(params.flags, 'i', FLAG_LEN)) < 0)
    des_reading(0, &params, 64);
    else if (((ft_strcmp(params.cipher, "des") == 0) || (ft_strcmp(params.cipher, "des-ecb") == 0)
  || (ft_strcmp(params.cipher, "des-cbc") == 0)) && (find_symb(params.flags, 'd', FLAG_LEN) >= 0)
&& (find_symb(params.flags, 'i', FLAG_LEN)) >= 0)
    des_reading(params.ifd, &params, 64);
  if (params.ifd > 1)
    close(params.ifd);
  if (params.ofd > 1)
    close(params.ofd);
}

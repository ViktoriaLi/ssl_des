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

int b64_save_flags(int argc, char **all_flags, char **argv, t_args *params)
{
  t_addition	iters;

  clear_iterators(&iters);
  iters.i = 2;
  while (iters.i < argc)
  {
    if (ft_strcmp(argv[iters.i], "-e") == 0 || ft_strcmp(argv[iters.i], "-d") == 0 ||
    ft_strcmp(argv[iters.i], "-i") == 0 || ft_strcmp(argv[iters.i], "-o") == 0)
    {
      (*all_flags)[iters.j++] = argv[iters.i][1];
      if (argv[iters.i][1] == 'i')
        (*params).ifd = open(argv[iters.i++], O_RDONLY);
      else if (argv[iters.i][1] == 'o')
      {
        (*params).ofd = open(argv[iters.i + 1], O_WRONLY | O_APPEND | O_CREAT, 0666);
        iters.i++;
      }
      iters.i++;
    }
    else
    {
      ft_printf("base64: invalid option -- %c\n", argv[iters.i][1]);
      return (0);
    }
  }
  return (1);
}

int check_b64_flags(int argc, char **argv, t_args *params)
{
  char *all_flags;

  if (!(all_flags = (char *)malloc(argc - 1)))
		return (1);
  all_flags[argc - 1] = 0;
  if (!b64_save_flags(argc, &all_flags, argv, params))
    return (1);
  flags_normalize(all_flags, params, argc - 1);
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

void save_des_flags(char **all_flags, char **argv, t_args *params, t_addition	*iters)
{
  (*all_flags)[(*iters).j] = argv[(*iters).i][1];
  if (argv[(*iters).i][1] == 'i')
  {
    (*params).ifd = open(argv[(*iters).i + 1], O_RDONLY);
    (*iters).i++;
  }
  if (argv[(*iters).i][1] == 'o')
  {
    (*params).ofd = open(argv[(*iters).i + 1], O_WRONLY | O_APPEND | O_CREAT, 0666);
    (*iters).i++;
  }
  if (argv[(*iters).i][1] == 'k')
  {
    (*params).des_key = (unsigned char *)argv[(*iters).i + 1];
    (*iters).i++;
  }
  if (argv[(*iters).i][1] == 'v')
  {
    (*params).vector16 = (unsigned char *)argv[(*iters).i + 1];
    (*iters).i++;
  }
  (*iters).j++;
  (*iters).i++;
}

int if_correct_des_flag(char *flag)
{
  if (ft_strcmp(flag, "-e") == 0 || ft_strcmp(flag, "-d") == 0 ||
  ft_strcmp(flag, "-i") == 0 || ft_strcmp(flag, "-o") == 0 || ft_strcmp(flag, "-a") == 0
  || ft_strcmp(flag, "-k") == 0 || ft_strcmp(flag, "-v") == 0 || ft_strcmp(flag, "-s") == 0
 || ft_strcmp(flag, "-p") == 0)
    return (1);
  return (0);
}

int check_des_flags(int argc, char **argv, t_args *params)
{
  t_addition	iters;

  char *all_flags;
  clear_iterators(&iters);
  iters.i = 2;
  if (!(all_flags = (char *)malloc(argc - 1)))
		return (1);
  all_flags[argc - 1] = 0;
  while (iters.i < argc)
  {
    if (if_correct_des_flag(argv[iters.i]))
      save_des_flags(&all_flags, argv, params, &iters);
    else
    {
      ft_printf("ft_ssl:Error: '%s' is an invalid command.\n", argv[iters.i]);
      return (1);
    }
  }
  flags_normalize(all_flags, params, argc - 1);
  if (((find_symb((*params).flags, 'i', FLAG_LEN)) >= 0 && (*params).ifd == -1)
  || ((find_symb((*params).flags, 'o', FLAG_LEN)) >= 0 && (*params).ofd == -1))
  {
    ft_printf("usage: ft_ssl %s [-i file] [-v IV] [-k key] [-o file]\n", argv[1]);
    return (1);
  }
  return (0);
}

int if_valid_args_des(int argc, char **argv, t_args *params, int res)
{
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

void md5_main(t_args *params, t_addition	*iters, int argc, char **argv)
{
	int			i;
	int			len;

	(*params).cipher = argv[1];
	if (ft_strcmp((*params).cipher, "sha512") == 0)
		len = 128;
	else
		len = 64;
	vectors_initiation(params, iters);
	reading_cases(params, iters, len);
	i = (*params).if_no_file;
	if (i)
		while (i < argc)
		{
			ft_printf("md5: %s: No such file or directory\n", argv[i]);
			i++;
		}
}

void des_main(t_args *params)
{
  int fd;
  int len;

  fd = 0;
  len = 48;
  if (find_symb((*params).flags, 'i', FLAG_LEN) >= 0)
    fd = (*params).ifd;
  if ((ft_strcmp((*params).cipher, "des") == 0 || ft_strcmp((*params).cipher, "des-ecb") == 0
|| ft_strcmp((*params).cipher, "des-cbc") == 0 || ft_strcmp((*params).cipher, "base64") == 0)
&& find_symb((*params).flags, 'd', FLAG_LEN) >= 0)
    len = 64;
  if (ft_strcmp((*params).cipher, "base64") == 0)
      base64_reading(fd, params, len);
  if (ft_strcmp((*params).cipher, "des") == 0 || ft_strcmp((*params).cipher, "des-ecb") == 0
|| ft_strcmp((*params).cipher, "des-cbc") == 0)
    des_reading(fd, params, len);
}

int main (int argc, char **argv)
{
  t_args params;
  t_addition	iters;

  clear_struct(&params);
  clear_iterators(&iters);
  if (ft_strcmp(argv[1], "md5") == 0 || ft_strcmp(argv[1], "sha256") == 0
	|| ft_strcmp(argv[1], "sha512") == 0)
  {
    if (!if_valid_args(argc, argv, &params, &iters))
  		return (0);
  }
  else
  {
    if (!if_valid_args_des(argc, argv, &params, 0))
      return (0);
  }
  if (ft_strcmp(argv[1], "md5") == 0 || ft_strcmp(argv[1], "sha256") == 0
  || ft_strcmp(argv[1], "sha512") == 0)
    md5_main(&params, &iters, argc, argv);
  else
    des_main(&params);
  if (params.ifd > 1)
    close(params.ifd);
  if (params.ofd > 1)
    close(params.ofd);
}

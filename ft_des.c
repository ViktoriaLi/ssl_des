/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_des.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/29 15:09:22 by vlikhotk          #+#    #+#             */
/*   Updated: 2018/03/29 15:09:25 by vlikhotk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <stdio.h>

void change_8bits(char **key_res)
{
  int i;
  int j;
  int bits_count;
  char tmp;

  i = 0;
  j = 0;
  tmp = 0;
  bits_count = 0;
  while (i < 8)
  {
    j = 0;
    tmp = (*key_res)[i];
    while (j < 8)
    {
      bits_count += tmp & 1;
      tmp >>= 1;
      j++;
    }
    if (bits_count % 2 == 0)
      (*key_res)[i] += 1;
    i++;
  }
}

void make_keys(t_argc *params)
{
  int i;
  int j;
  char *tmp;
  char *key_res;

  i = 0;
  j = 0;
  tmp = (char *)malloc(17);
  key_res = (char *)malloc(9);
  tmp[16] = 0;
  key_res[8] = 0;
  printf("%s\n", (*params).des_key);
  tmp = ft_strncpy(tmp, (*params).des_key, 16);
  printf("%s\n", tmp);
  while (i < 8)
  {
    if (tmp[i] >= 65 && tmp[i] <= 70)
    {
      tmp[i] = tmp[i] + 32 - 49;
    }
    else if (tmp[i] >= 97 && tmp[i] <= 102)
    {
      tmp[i] = tmp[i] - 49;
    }
    i++;
  }
  printf("%s\n", tmp);
  //key_res[0] = ( * 16) + (tmp[1] - '0');
  i = 0;
  while (i < 8)
  {
    key_res[i] = ((tmp[j] - '0') * 16) + (tmp[j + 1] - '0');
    i++;
    j += 2;
  }
  printf("%s\n", key_res);
  change_8bits(&key_res);
  //printf("%d\n", key_res[0]);
  printf("%s\n", key_res);
}

void des_dec(char *buf, t_argc *params)
{

}

void des_enc(char *buf, t_argc *params)
{
  make_keys(params);
}

void des_read(t_argc *params, char **argv)
{
  int i;
  int ret;
  char buf[9];

  buf[8] = 0;
  ret = 0;
  if ((find_symb((*params).flags, 'i', FLAG_LEN)) >= 0)
  {
    while ((ret = read((*params).ifd, &buf, DES_BLOCK)) > 0)
    {
      ft_printf("%s\n", buf);
      if ((find_symb((*params).flags, 'd', FLAG_LEN)) >= 0)
        des_dec(buf, params);
      else
        des_enc(buf, params);
      if (ret < DES_BLOCK)
        break;
      i = 0;
      while (i < DES_BLOCK)
      {
        buf[i] = 0;
        i++;
      }
    }
  }
  else if ((ret = read(0, &buf, DES_BLOCK)) > 0)
  {
    ft_printf("%s\n", buf);
    if ((find_symb((*params).flags, 'd', FLAG_LEN)) >= 0)
      des_dec(buf, params);
    else
      des_enc(buf, params);
    /*else if (ft_strcmp(argv[1], "des") == 0)
      des_enc(buf);
    else if (ft_strcmp(argv[1], "des-ecb") == 0)
      des_ecb_enc(buf);
    else if (ft_strcmp(argv[1], "des-cbc") == 0)
      des_cbc_enc(buf);*/
    i = 0;
    while (i < DES_BLOCK)
    {
      buf[i] = 0;
      i++;
    }
    while ((ret = read(0, &buf, DES_BLOCK)) > 0)
    {
      ft_printf("%s\n", buf);
      if ((find_symb((*params).flags, 'd', FLAG_LEN)) >= 0)
        des_dec(buf, params);
      else
        des_enc(buf, params);
      if (ret < DES_BLOCK)
        break;
      i = 0;
      while (i < DES_BLOCK)
      {
        buf[i] = 0;
        i++;
      }
    }
  }
  else if ((ret = read(1, &buf, DES_BLOCK)) > 0)
  {
      ft_printf("%s\n", buf);
      if ((find_symb((*params).flags, 'd', FLAG_LEN)) >= 0)
        des_dec(buf, params);
      else
        des_enc(buf, params);
      i = 0;
      while (i < DES_BLOCK)
      {
        buf[i] = 0;
        i++;
      }
      while ((ret = read(1, &buf, DES_BLOCK)) > 0)
      {
        ft_printf("%s\n", buf);
        if ((find_symb((*params).flags, 'd', FLAG_LEN)) >= 0)
          des_dec(buf, params);
        else
          des_enc(buf, params);
        if (ret < DES_BLOCK)
          break;
        i = 0;
        while (i < DES_BLOCK)
        {
          buf[i] = 0;
          i++;
        }
      }
  }
  if ((find_symb((*params).flags, 'o', FLAG_LEN)) < 0)
    ft_printf("%s", "\n");
}

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

void finish_key_shift(char *key_res[])
{
   int i;
  int j;
  char key_48[6];
  

  i = 0;
  j = 0; 
  while (i < 6)
  {
    key_48[i] = 0;
    i++;
  }
  i = 0;
  while (i < 6)
  {
    j = 7;
    while (j >= 0)
    {
      key_48[i] |= ((1 << (KEY_FINISH[i] % 8) - 1 ) & 
        key_res[(KKEY_FINISH[i] - KEY_FINISH[i] % 8) / 8]) << j;
      j--
    }
    i++;
  }


}

void two_bit_shift(char *key_res[])
{
  int bit0;
  int bit1;

  bit0 = 0;
  bit1 = 0;
  bit0 = *key_res[0] >> 7;
  bit1 = (*key_res[0] << 1) + (*key_res[0] >> 6);
  *key_res[0] = ((*key_res[0] & 252) << 2) + (*key_res[1] >> 6);
  *key_res[1] = ((*key_res[1] & 252) << 2) + (*key_res[2] >> 6);
  *key_res[2] = ((*key_res[2] & 252) << 2) + (*key_res[3] >> 6);
  *key_res[3] = ((*key_res[3] & 252) << 2);
  if (bit0 == 1)
    *key_res[3] |= (1 << 6);
  else
    *key_res[3] &= ~(1 << 6);
  if (bit1 == 1)
    *key_res[3] |= (1 << 5);
  else
    *key_res[3] &= ~(1 << 5);
  bit = ( key_res[3] >> 3 ) + (*key_res[3] << 4);
  *key_res[4] = ((*key_res[4] & 252) << 2) + (*key_res[5] >> 6);
  *key_res[5] = ((*key_res[5] & 252) << 2) + (*key_res[6] >> 6);
  *key_res[6] = ((*key_res[6] & 252) << 2);
  if (bit == 1)
    *key_res[6] |= (1 << 1);
  else
    *key_res[6] &= ~(1 << 1);
  if (bit1 == 1)
    *key_res[6] |= (1 << 0);
  else
    *key_res[6] &= ~(1 << 0);
}

void one_bit_shift(char *key_res[])
{
  int bit;

  bit = 0;
  bit = *key_res[0] >> 7;
  *key_res[0] = ((*key_res[0] & 254) << 1) + (*key_res[1] >> 7);
  *key_res[1] = ((*key_res[1] & 254) << 1) + (*key_res[2] >> 7);
  *key_res[2] = ((*key_res[2] & 254) << 1) + (*key_res[3] >> 7);
  *key_res[3] = ((*key_res[3] & 254) << 1);
  if (bit == 1)
    *key_res[3] |= (1 << 5);
  else
    *key_res[3] &= ~(1 << 5);
  bit =( key_res[3] >> 3 ) + (*key_res[3] << 4);
  *key_res[4] = ((*key_res[4] & 254) << 1) + (*key_res[5] >> 7);
  *key_res[5] = ((*key_res[5] & 254) << 1) + (*key_res[6] >> 7);
  *key_res[6] = ((*key_res[6] & 254) << 1);
  if (bit == 1)
    *key_res[6] |= (1 << 0);
  else
    *key_res[6] &= ~(1 << 0);
}

void remove_8bits(char key_res[])
{
  int i;
  int j;
  char key_56[7];
  

  i = 0;
  j = 0;
  
  while (i < 7)
  {
    key_56[i] = 0;
    i++;
  }
  i = 0;
  /*key_56[0] = ((key_res[0] & 254) << 1) + (key_res[1] >> 7);
  key_56[1] = ((key_res[1] & (254 << 2)) << 1) + (key_res[2] >> 6);
  key_56[2] = ((key_res[2] & (254 << 3)) << 2) + (key_res[3] >> 5);
  key_56[3] = ((key_res[3] & (254 << 4)) << 3) + (key_res[4] >> 4);
  key_56[4] = ((key_res[4] & (254 << 5)) << 4) + (key_res[5] >> 3);
  key_56[5] = ((key_res[5] & (254 << 6)) << 5) + (key_res[6] >> 2);
  key_56[6] = ((key_res[6] & (254 << 7)) << 6) + (key_res[7] >> 1);*/
  while (i < 7)
  {
    j = 7;
    while (j >= 0)
    {
      key_56[i] |= ((1 << (KEY_START[i] % 8) - 1 ) & 
        key_res[(KEY_START[i] - KEY_START[i] % 8) / 8]) << j;
      j--
    }
    i++;
  }
  one_bit_shift(&key_res);
 // first_key_shift(key_56);
}

void make_keys(t_argc *params)
{
  int i;
  int j;
  char key_res[8];

  i = 0;
  j = 0;
  printf("%s\n", (*params).des_key);
  while (i < 8)
    key_res[i++] = 0;
  //key_res[0] = ( * 16) + (tmp[1] - '0');
  i = 0;
  while (i < 8 && (*params).des_key[j])
  {
    if ((*params).des_key[j] >= 65 && (*params).des_key[j] <= 70)
    {
      (*params).des_key[j] = (*params).des_key[j] + 32 - 49;
    }
    else if ((*params).des_key[j] >= 97 && (*params).des_key[j] <= 102)
    {
      (*params).des_key[j] = (*params).des_key[j] - 49;
    }
    key_res[i] = (((*params).des_key[j] - '0') * 16) + ((*params).des_key[j + 1] - '0');
    i++;
    j += 2;
  }
  printf("%s\n", key_res);
  remove_8bits(key_res);
  //printf("%d\n", key_res[0]);
  printf("%s\n", key_res);
}

void des_dec(char buf[], t_argc *params)
{

}

void des_enc(char buf[], t_argc *params)
{
  make_keys(params);
  char first_shift[64];
  char left[4];
  char right[4];
  
}

void des_read(t_argc *params, char **argv)
{
  int i;
  int ret;
  char buf[8];

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
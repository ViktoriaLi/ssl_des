/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/28 17:16:44 by vlikhotk          #+#    #+#             */
/*   Updated: 2018/03/28 17:16:47 by vlikhotk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void base64_dec(unsigned char *buf, t_args *params)
{
  unsigned char dest[4];
  unsigned char tmp[4];
  int j;
	int len;

  j = 0;
  dest[0] = find_symb(BASE64_STR, buf[0], 64);
  dest[1] = find_symb(BASE64_STR, buf[1], 64);
  if (buf[2] != '=')
    dest[2] = find_symb(BASE64_STR, buf[2], 64);
  if (buf[3] != '=')
    dest[3] = find_symb(BASE64_STR, buf[3], 64);
  tmp[0] = (dest[0] << 2) + (dest[1] >> 4);
  len = 1;
  if (buf[2] != '=')
  {
    tmp[1] = (dest[1] << 4) + (dest[2] >> 2);
    len = 2;
  }
  if (buf[3] != '=')
  {
    tmp[2] = (dest[2] << 6) + dest[3];
    len = 3;
  }
  if (ft_strcmp((*params).cipher, "base64") == 0)
  {
    if ((find_symb((*params).flags, 'o', FLAG_LEN)) >= 0)
      write((*params).ofd, tmp, len);
    else
      write(1, (char *)tmp, len);
  }
  else
  {
    j = 0;
    while (j < len)
      (*params).des_48_res[(*params).desad_count++] = tmp[j++];
    //printf("%s\n", );
  }
}

void base64_enc(unsigned char *buf, t_args *params, int j)
{
  unsigned char dest[4];

  dest[2] = '=';
  dest[3] = '=';
  dest[0] = BASE64_STR[buf[0] >> 2];
  dest[1] = BASE64_STR[((buf[0] & 3) << 4) + ((buf[1]) >> 4)];
  if (j > 1)
    dest[2] = BASE64_STR[((buf[1] & 15) << 2) + ((buf[2]) >> 6)];
  if (j > 2)
    dest[3] = BASE64_STR[buf[2] & 63];
  if ((find_symb((*params).flags, 'o', FLAG_LEN)) >= 0)
    write((*params).ofd, dest, 4);
  else
    write(1, dest, 4);
}

void base64_read(t_args *params, char **argv, int len)
{
  int i;
	int j;
  int ret;
  static unsigned char tmpb64d[4];

  if ((find_symb((*params).flags, 'i', FLAG_LEN)) >= 0)
  {
    while ((ret = read((*params).ifd, &params->b64_buf, len)) > 0)
    {
			i = 0;
			if (len == 65 && ret == len)
				ret -= 1;
      (*params).b64_buf[64] = 0;
      //ft_printf("%s\n", (*params).b64_buf);
			//ft_printf("%d\n", ret);
      if (find_symb((*params).flags, 'd', FLAG_LEN) >= 0)
      {
				//(*params).b64_buf[64] = 0;
        while (i < ret - 1)
        {
					j = 0;
					while (j < 4 && i < ret)
						tmpb64d[j++] = (*params).b64_buf[i++];
					//ft_printf("%s\n", tmpb64d);
          base64_dec(tmpb64d, params);
					j = 0;
					while (j < 4)
						tmpb64d[j++] = 0;
        }
      }
      else
      {
				i = 0;
        while (i < ret)
        {
					j = 0;
					while (j < 3 && i < ret)
						tmpb64d[j++] = (*params).b64_buf[i++];
          base64_enc(tmpb64d, params, j);
					j = 0;
		      while (j < 3)
		      {
		        tmpb64d[j] = 0;
		        j++;
		      }
        }
      }
      i = 0;
      while (i < ret)
      {
        (*params).b64_buf[i] = 0;
        i++;
      }
      if (ret == 64 && (find_symb((*params).flags, 'd', FLAG_LEN)) < 0 && (find_symb((*params).flags, 'o', FLAG_LEN)) >= 0)
        write((*params).ofd, "\n", 1);
      else if (ret == 64 && (find_symb((*params).flags, 'd', FLAG_LEN)) < 0)
        write(1, "\n", 1);
    }
  }
  else
  {
    while ((ret = read(0, &params->b64_buf, len)) > 0)
    {
			i = 0;
      //ft_printf("%s\n", (*params).b64_buf);
			//ft_printf("%d\n", ret);
			//ft_printf("2%s\n", (*params).b64_buf);
			if (find_symb((*params).flags, 'd', FLAG_LEN) >= 0)
			{
				while (i < ret - 1)
				{
					j = 0;
					while (i < ret && j < 4)
						tmpb64d[j++] = (*params).b64_buf[i++];
					base64_dec(tmpb64d, params);
					j = 0;
					while (j < 4)
						tmpb64d[j++] = 0;
					}
			}
			else
			{
				i = 0;
				while (i < ret)
				{
					j = 0;
					while (i < ret && j < 3)
						tmpb64d[j++] = (*params).b64_buf[i++];
					//ft_printf("2%s\n", tmpb64d);
					base64_enc(tmpb64d, params, j);
					j = 0;
		      while (j < 3)
		      {
		        tmpb64d[j] = 0;
		        j++;
		      }
					}
			}
      i = 0;
      while (i < ret)
      {
        (*params).b64_buf[i] = 0;
        i++;
      }
      if (ret == 64 && (find_symb((*params).flags, 'd', FLAG_LEN)) < 0 && (find_symb((*params).flags, 'o', FLAG_LEN)) >= 0)
        write((*params).ofd, "\n", 1);
      else if (ret == 64 && (find_symb((*params).flags, 'd', FLAG_LEN)) < 0)
        write(1, "\n", 1);
    }
  }
}

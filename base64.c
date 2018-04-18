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

void ft_des_strsub(char const *s, unsigned int start, size_t len, char dest[])
{
	size_t	i;

	i = 0;
	if (!s || start > ft_strlen(s) || !(dest = (char *)malloc(sizeof(char) *
		(len + 1))))
		return (NULL);
	while (i < len && s[start] != 0)
	{
		dest[i] = (char)s[start];
		i++;
		start++;
	}
	dest[i] = 0;
}


void base64_dec(unsigned char *buf, t_args *params)
{
  unsigned char dest[4];
  unsigned char tmp[4];

  dest[0] = find_symb(BASE64_STR, buf[0], 64);
  //printf("%d\n", dest[1]);
  dest[1] = find_symb(BASE64_STR, buf[1], 64);;
  //printf("%d\n", dest[1]);
  if (buf[2] != '=')
  {
    dest[2] = find_symb(BASE64_STR, buf[2], 64);;
    //printf("%d\n", dest[1]);
  }
  if (buf[3] != '=')
  {
    dest[3] = find_symb(BASE64_STR, buf[3], 64);;
    //printf("%d\n", dest[1]);
  }
  tmp[0] = (dest[0] << 2) + (dest[1] >> 4);
  if (buf[2] != '=')
    tmp[1] = (dest[1] << 4) + (dest[2] >> 2);
  if (buf[3] != '=')
    tmp[2] = (dest[2] << 6) + dest[3];
  if ((find_symb((*params).flags, 'o', FLAG_LEN)) >= 0)
    write((*params).ofd, tmp, ft_strlen((char *)tmp));
  else
    write(1, (char *)tmp, ft_strlen((char *)tmp));
}

void base64_enc(unsigned char *buf, t_args *params)
{
  unsigned char dest[5];

  dest[2] = '=';
  dest[3] = '=';
  dest[4] = 0;
  dest[0] = BASE64_STR[buf[0] >> 2];
  dest[1] = BASE64_STR[((buf[0] & 3) << 4) + ((buf[1]) >> 4)];
  if (ft_strlen((char *)buf) > 1)
    dest[2] = BASE64_STR[((buf[1] & 15) << 2) + ((buf[2]) >> 6)];
  if (ft_strlen((char *)buf) > 2)
    dest[3] = BASE64_STR[buf[2] & 63];
  if ((find_symb((*params).flags, 'o', FLAG_LEN)) >= 0)
    write((*params).ofd, dest, 4);
  else
    write(1, dest, 4);
}

void base64_read(t_args *params, char **argv, int len)
{
  int i;
  int ret;
  static char tmpb64e[3];
  static char tmpb64d[4];

  i = 0;
  ret = 0;
  if ((find_symb((*params).flags, 'i', FLAG_LEN)) >= 0)
  {
    while ((ret = read((*params).ifd, &params->b64_buf, len)) > 0)
    {
      //ft_printf("%s\n", buf);
      if (params.flags, 'd', FLAG_LEN) >= 0)
      {
        while (i < len)
        {
          ft_strsub((*params).b64_buf, i, 4, tmpb64d);
          base64_dec(tmpb64d, params);
          i += 4;
        }
      }
      else
      {
        while (i < len)
        {
          ft_strsub((*params).b64_buf, i, 3, tmpb64e);
          base64_enc(tmpb64e, params);
          i += 3;
        }
      }
      i = 0;
      while (i < len)
      {
        (*params).b64_buf[i] = 0;
        i++;
      }
    }
  }
  else if ((ret = read(0, &params->b64_buf, len)) > 0)
  {
    //ft_printf("%s\n", buf);
    if (params.flags, 'd', FLAG_LEN) >= 0)
      base64_dec((*params).b64_buf, params);
    else
      base64_enc((*params).b64_buf, params);
    /*else if (ft_strcmp(argv[1], "des") == 0)
      des_enc(buf);
    else if (ft_strcmp(argv[1], "des-ecb") == 0)
      des_ecb_enc(buf);
    else if (ft_strcmp(argv[1], "des-cbc") == 0)
      des_cbc_enc(buf);*/
    i = 0;
    while (i < len)
    {
      (*params).b64_buf[i] = 0;
      i++;
    }
    while ((ret = read(0, &params->b64_buf, len)) > 0)
    {
      //ft_printf("%s\n", buf);
      if (params.flags, 'd', FLAG_LEN) >= 0)
        base64_dec((*params).b64_buf, params);
      else
        base64_enc((*params).b64_buf, params);
      i = 0;
      while (i < len)
      {
        (*params).b64_buf[i] = 0;
        i++;
      }
    }
  }
  else if ((ret = read(1, &params->b64_buf, len)) > 0)
  {
      //ft_printf("%s\n", buf);
      if (params.flags, 'd', FLAG_LEN) >= 0)
        base64_dec((*params).b64_buf, params);
      else
        base64_enc((*params).b64_buf, params);
      i = 0;
      while (i < len)
      {
        (*params).b64_buf[i] = 0;
        i++;
      }
      while ((ret = read(1, &params->b64_buf, len)) > 0)
      {
        //ft_printf("%s\n", buf);
        if (params.flags, 'd', FLAG_LEN) >= 0)
          base64_dec((*params).b64_buf, params);
        else
          base64_enc((*params).b64_buf, params);
        i = 0;
        while (i < len)
        {
          (*params).b64_buf[i] = 0;
          i++;
        }
      }
  }
  if (len == 3 && (find_symb((*params).flags, 'o', FLAG_LEN)) < 0)
    ft_printf("%s", "\n");
}

/*void base64_dec(unsigned char *buf, t_args *params)
{
  unsigned char dest[5];
  unsigned char tmp[4];

  dest[4] = 0;
  tmp[0] = 0;
  tmp[1] = 0;
  tmp[2] = 0;
  tmp[3] = 0;
  dest[0] = find_symb(BASE64_STR, buf[0], 64);
  //printf("%d\n", dest[1]);
  dest[1] = find_symb(BASE64_STR, buf[1], 64);;
  //printf("%d\n", dest[1]);
  if (buf[2] != '=')
  {
    dest[2] = find_symb(BASE64_STR, buf[2], 64);;
    //printf("%d\n", dest[1]);
  }
  if (buf[3] != '=')
  {
    dest[3] = find_symb(BASE64_STR, buf[3], 64);;
    //printf("%d\n", dest[1]);
  }
  tmp[0] = (dest[0] << 2) + (dest[1] >> 4);
  if (buf[2] != '=')
    tmp[1] = (dest[1] << 4) + (dest[2] >> 2);
  if (buf[3] != '=')
    tmp[2] = (dest[2] << 6) + dest[3];
  if ((find_symb((*params).flags, 'o', FLAG_LEN)) >= 0)
    write((*params).ofd, tmp, ft_strlen((char *)tmp));
  else
    write(1, (char *)tmp, ft_strlen((char *)tmp));
}

void base64_enc(unsigned char *buf, t_args *params)
{
  unsigned char dest[4];

  dest[2] = '=';
  dest[3] = '=';
  dest[0] = BASE64_STR[buf[0] >> 2];
  dest[1] = BASE64_STR[((buf[0] & 3) << 4) + ((buf[1]) >> 4)];
  if (ft_strlen((char *)buf) > 1)
    dest[2] = BASE64_STR[((buf[1] & 15) << 2) + ((buf[2]) >> 6)];
  if (ft_strlen((char *)buf) > 2)
    dest[3] = BASE64_STR[buf[2] & 63];
  if ((find_symb((*params).flags, 'o', FLAG_LEN)) >= 0)
    write((*params).ofd, dest, 4);
  else
    write(1, dest, 4);
}

void base64_read(t_args *params, char **argv, int len)
{
  int i;
  int ret;
  unsigned char buf[4];

  ret = 0;
  if ((find_symb((*params).flags, 'i', FLAG_LEN)) >= 0)
  {
    while ((ret = read((*params).ifd, &buf, len)) > 0)
    {
      //ft_printf("%s\n", buf);
      i = 0;
      while (i < len)
      {
        buf[i] = 0;
        i++;
      }
      if (len == 4)
        base64_dec(buf, params);
      else
        base64_enc(buf, params);
      if ((len == 4 && buf[3] == '=') || (len == 3 && ret < 3))
        break;
    }
  }
  else if ((ret = read(0, &buf, len)) > 0)
  {
    //ft_printf("%s\n", buf);
    i = 0;
    while (i < len)
    {
      buf[i] = 0;
      i++;
    }
    if (len == 4)
      base64_dec(buf, params);
    else
      base64_enc(buf, params);
    //else if (ft_strcmp(argv[1], "des") == 0)
      //des_enc(buf);
    //else if (ft_strcmp(argv[1], "des-ecb") == 0)
    //  des_ecb_enc(buf);
    //else if (ft_strcmp(argv[1], "des-cbc") == 0)
    //  des_cbc_enc(buf);
    while ((ret = read(0, &buf, len)) > 0)
    {
      //ft_printf("%s\n", buf);
      i = 0;
      while (i < len)
      {
        buf[i] = 0;
        i++;
      }
      if (len == 4)
        base64_dec(buf, params);
      else
        base64_enc(buf, params);
      if ((len == 4 && buf[3] == '=') || (len == 3 && ret < 3))
        break;
    }
  }
  else if ((ret = read(1, &buf, len)) > 0)
  {
      //ft_printf("%s\n", buf);
      i = 0;
      while (i < len)
      {
        buf[i] = 0;
        i++;
      }
      if (len == 4)
        base64_dec(buf, params);
      else
        base64_enc(buf, params);

      while ((ret = read(1, &buf, len)) > 0)
      {
        //ft_printf("%s\n", buf);
        i = 0;
        while (i < len)
        {
          buf[i] = 0;
          i++;
        }
        if (len == 4)
          base64_dec(buf, params);
        else
          base64_enc(buf, params);
        if ((len == 4 && buf[3] == '=') || (len == 3 && ret < 3))
          break;
      }
  }
  if (len == 3 && (find_symb((*params).flags, 'o', FLAG_LEN)) < 0)
    ft_printf("%s", "\n");
}*/

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
	int len;

	len = 0 ;
	dest[3] = 0;
  tmp[0] = 0;
  tmp[1] = 0;
  tmp[2] = 0;
  tmp[3] = 0;
  dest[0] = find_symb(BASE64_STR, buf[0], 64);
  //printf("%d\n", dest[1]);
  dest[1] = find_symb(BASE64_STR, buf[1], 64);
	len = 1;
  //printf("%d\n", dest[1]);
  if (buf[2] != '=')
  {
    dest[2] = find_symb(BASE64_STR, buf[2], 64);
		len = 1;
    //printf("%d\n", dest[1]);
  }
  if (buf[3] != '=')
  {
    dest[3] = find_symb(BASE64_STR, buf[3], 64);
		len = 3;
    //printf("%d\n", dest[1]);
  }
  tmp[0] = (dest[0] << 2) + (dest[1] >> 4);
  if (buf[2] != '=')
    tmp[1] = (dest[1] << 4) + (dest[2] >> 2);
  if (buf[3] != '=')
    tmp[2] = (dest[2] << 6) + dest[3];
  if ((find_symb((*params).flags, 'o', FLAG_LEN)) >= 0)
    write((*params).ofd, tmp, len);
  else
    write(1, (char *)tmp, len);
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

  i = 0;
	j = 0;
  ret = 0;
	tmpb64d[0] = 0;
	tmpb64d[1] = 0;
	tmpb64d[2] = 0;
	tmpb64d[3] = 0;
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
    }
  }
  else
  {
    while ((ret = read(0, &params->b64_buf, len)) > 0)
    {
			i = 0;
      if (len == 65 && ret == len)
  			ret -= 1;
      (*params).b64_buf[64] = 0;
      //ft_printf("%s\n", (*params).b64_buf);
			//ft_printf("%d\n", ret);
			//ft_printf("2%s\n", (*params).b64_buf);
			if (find_symb((*params).flags, 'd', FLAG_LEN) >= 0)
			{
				//(*params).b64_buf[64] = 0;
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
    }
  }
  /*else if ((ret = read(1, &params->b64_buf, len)) > 0)
  {
      //ft_printf("%s\n", (*params).b64_buf);
			if (find_symb((*params).flags, 'd', FLAG_LEN) >= 0)
			{
				while (i < ret)
				{
					j = 0;
					while (i < ret && j < 4)
						tmpb64d[j++] = (*params).b64_buf[i++];
					base64_dec(tmpb64d, params);
					j = 0;
					while (j < 4)
					{
						tmpb64d[j] = 0;
						j++;
					}
				}
			}
			else
			{
				while (i < ret)
				{
					j = 0;
					while (i < ret && j < 3)
						tmpb64d[j++] = (*params).b64_buf[i++];
					base64_enc(tmpb64d, params);
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
      while ((ret = read(1, &params->b64_buf, len)) > 0)
      {
        //ft_printf("%s\n", (*params).b64_buf);
				if (find_symb((*params).flags, 'd', FLAG_LEN) >= 0)
				{
					while (i < ret - 1 )
					{
						j = 0;
						while (i < ret && j < 4)
							tmpb64d[j++] = (*params).b64_buf[i++];
						base64_dec(tmpb64d, params);
						j = 0;
						while (j < 4)
						{
							tmpb64d[j] = 0;
							j++;
						}
					}
				}
				else
				{
					while (i < ret)
					{
						j = 0;
						while (i < ret && j < 3)
							tmpb64d[j++] = (*params).b64_buf[i++];
						base64_enc(tmpb64d, params);
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
      }
  }*/
  /*if (len == 3 && (find_symb((*params).flags, 'o', FLAG_LEN)) < 0)
    ft_printf("%s", "\n");*/
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

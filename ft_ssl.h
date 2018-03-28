/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/22 13:15:59 by vlikhotk          #+#    #+#             */
/*   Updated: 2018/03/01 18:21:51 by vlikhotk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <pwd.h>

# define BASE64_STR "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

typedef struct		s_argc
{
	char	*cipher;
  char *buf;
	char flags[6];
	int ifd;
	int ofd;
}					t_argc;

int		get_next_line(const int fd, char **line);
size_t		ft_strlen(const char *s);
int		ft_printf(const char *format, ...);
int		ft_strcmp(const char *s1, const char *s2);
int find_symb(char *str, char flag, int len);
void base64_enc(char *buf, t_argc *params);
void base64_dec(char *buf, t_argc *params);
void base64_read(t_argc *params, char **argv, int len);
void flags_normalize(char *all_flags, t_argc *params, int len);
int check_ssl_flags(int argc, char **argv, t_argc *params);
int if_valid_args(int argc, char **argv, t_argc *params);
void clear_struct(t_argc *params);

#endif

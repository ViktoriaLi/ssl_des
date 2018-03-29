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
# define FLAG_LEN 7
# define DES_BLOCK 8
# define DES_PC_1 {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27,\
	19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53,\
	45, 37, 29, 21, 13, 5, 28, 20, 12, 4}

typedef struct		s_argc
{
	char	*cipher;
  char *buf;
	char flags[FLAG_LEN];
	int ifd;
	int ofd;
	char *des_key;
	char *des_key_res;
}					t_argc;

int		get_next_line(const int fd, char **line);
size_t		ft_strlen(const char *s);
int		ft_printf(const char *format, ...);
int		ft_strcmp(const char *s1, const char *s2);
int find_symb(char *str, char flag, int len);
void base64_enc(char *buf, t_argc *params);
void base64_dec(char *buf, t_argc *params);
void base64_read(t_argc *params, char **argv, int len);
void des_read(t_argc *params, char **argv);
void flags_normalize(char *all_flags, t_argc *params, int len);
int check_b64_flags(int argc, char **argv, t_argc *params);
int if_valid_args(int argc, char **argv, t_argc *params);
void clear_struct(t_argc *params);
int check_des_flags(int argc, char **argv, t_argc *params);

void des_dec(char *buf, t_argc *params);
void des_enc(char *buf, t_argc *params);
char	*ft_strncpy(char *dst, const char *src, size_t len);

#endif

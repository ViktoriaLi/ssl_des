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
# define FLAG_LEN 15
# define DES_BLOCK 8
# define KEY_LEN 8

typedef struct		s_args
{
	char	*cipher;
	int if_full;
	int desad_count;
  unsigned char b64_buf[65];
  unsigned char buf[DES_BLOCK];
  unsigned char *vector16;
	char flags[FLAG_LEN];
	int ifd;
	int ofd;
	unsigned char *des_key;
	unsigned char key_res48[6];
	unsigned char key_res56[7];
	unsigned char des_output[8];
	unsigned char des_48_read[65];
	unsigned char des_48_res[65];
}					t_args;

typedef struct		s_addition
{
  int i;
  int j;
  int k;
  int m;
	unsigned char str1[4];
	unsigned char str2[4];
}					t_addition;


int		ft_printf(const char *format, ...);
int		ft_strcmp(const char *s1, const char *s2);
int find_symb(char *str, char flag, int len);
void base64_enc(unsigned char *buf, t_args *params, int j);
void base64_dec(unsigned char *buf, t_args *params);
void flags_normalize(char *all_flags, t_args *params, int len);
int check_b64_flags(int argc, char **argv, t_args *params);
int if_valid_args(int argc, char **argv, t_args *params);
void clear_struct(t_args *params);
int check_des_flags(int argc, char **argv, t_args *params);
void des_read(t_args *params, char **argv, int len);
void des_enc(t_args *params, int count, int *l);
void	to_binary(int **res, int nbr, unsigned int base);
void finish_key_shift(unsigned char key_56[], t_args *params);
void remove_8bits(unsigned char key_res[], t_args *params, int rounds);
void one_bit_shift_right(unsigned char key_56[], t_args *params);
void two_bit_shift_right(unsigned char key_56[], t_args *params);
void one_bit_shift_left(unsigned char key_56[], t_args *params);
void two_bit_shift_left(unsigned char key_56[], t_args *params);
void bit_permutations(int max, const int table[], unsigned char key_56[], unsigned char *src);
void clear_iterators(t_addition *iters);
void	print_or_save_res(t_args *params, unsigned char *str, int len);
void	reading(int fd, t_args *params, int len);
void make_keys(unsigned char **des_key, t_args *params, int rounds);
void message_first_shift(t_args *params);

size_t		ft_strlen(const char *s);
void des_dec(t_args *params, int count);
char	*ft_strncpy(char *dst, const char *src, size_t len);

#endif

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
# define KEY_LEN 8

typedef struct		s_args
{
	//char	*cipher;
  unsigned char buf[DES_BLOCK];
	char flags[FLAG_LEN];
	int ifd;
	int ofd;
	unsigned char *des_key;
	unsigned char key_res48[6];
	unsigned char key_res56[7];
	unsigned char des_output[8];
}					t_args;

/*typedef struct		s_des_tables
{
	int m_start[64];
	int m_end[64];
	int r_to_48[48];
	int key_start[56];
	int shift_table_e[16];
	int shift_table_d[16];
	int key_finish[48];
	int s_1[64];
	int s_2[64];
	int s_3[64];
	int s_4[64];
	int s_5[64];
	int s_6[64];
	int s_7[64];
	int s_8[64];
	int shift_32bits[32];
	int p_shift[32];
}					t_des_tables;*/

int		get_next_line(const int fd, char **line);
size_t		ft_strlen(const char *s);
int		ft_printf(const char *format, ...);
int		ft_strcmp(const char *s1, const char *s2);
int find_symb(char *str, char flag, int len);
void base64_enc(unsigned char *buf, t_args *params);
void base64_dec(unsigned char *buf, t_args *params);
void base64_read(t_args *params, char **argv, int len);

void flags_normalize(char *all_flags, t_args *params, int len);
int check_b64_flags(int argc, char **argv, t_args *params);
int if_valid_args(int argc, char **argv, t_args *params);
void clear_struct(t_args *params);
int check_des_flags(int argc, char **argv, t_args *params);

void des_read(t_args *params, char **argv);
void des_dec(t_args *params);
void des_enc(t_args *params);
void	to_binary(int **res, int nbr, unsigned int base);
void finish_key_shift(unsigned char key_56[], t_args *params);
void remove_8bits(unsigned char key_res[], t_args *params, int rounds);

void one_bit_shift(unsigned char key_56[], t_args *params);
void two_bit_shift(unsigned char key_56[], t_args *params);

char	*ft_strncpy(char *dst, const char *src, size_t len);

#endif

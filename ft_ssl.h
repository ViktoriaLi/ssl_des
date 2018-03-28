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

#endif

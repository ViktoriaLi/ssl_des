#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vlikhotk <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/10/27 15:23:08 by vlikhotk          #+#    #+#              #
#    Updated: 2017/11/02 15:15:21 by vlikhotk         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = ft_ssl

LIB_DIR = ./libft/

SFILES = main.c init_vectors.c md5.c sha256.c sha512.c printing.c \
 				 bits_shifting.c last_rounds_md5.c flags_ssl.c base64.c ft_des.c \
				 start_md5.c reading.c des_flags.c s_blocks.c s_blocks_proc.c \
				 des_two_bits_shifting.c des_one_bit_shift.c des_steps.c \
				 des_add_funcs.c des_add_funcs2.c des_reading.c b64_flags_spaces.c

OFILES = main.o init_vectors.o md5.o sha256.o sha512.o printing.o \
				 bits_shifting.o last_rounds_md5.o flags_ssl.o base64.o ft_des.o \
				 start_md5.o reading.o des_flags.o s_blocks.o s_blocks_proc.o \
				 des_two_bits_shifting.o des_one_bit_shift.o des_steps.o \
				 des_add_funcs.o des_add_funcs2.o des_reading.o b64_flags_spaces.o

LIBFT = $(LIBFT_DIR)libftprintf.a
LIBFT_DIR = $(LIB_DIR)
LIBFT_INC = $(LIBFT_DIR)includes/

CC = gcc
CC_FLAGS = -Wall -Wextra -Werror
HEADER_FLAGS = -I $(LIBFT_INC)


all: $(NAME)

$(NAME): $(LIBFT) $(OFILES)
	$(CC) $(OFILES) $(LIBFT) -o $(NAME)

$(OFILES): %.o:%.c
	$(CC) -c $< -o $@ $(CC_FLAGS) $(HEADER_FLAGS)

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -rf $(OFILES)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -rf $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

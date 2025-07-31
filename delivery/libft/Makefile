# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/27 13:09:33 by djunho            #+#    #+#              #
#    Updated: 2025/07/31 23:55:56 by bfiochi-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Werror -Wextra -ggdb
NAME = libft.a
SRCS = ft_bzero.c ft_isalpha.c ft_isdigit.c ft_memcpy.c	ft_memset.c \
		ft_strlen.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_memmove.c \
		ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
		ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
		ft_atoi.c ft_atol.c ft_atoll.c ft_calloc.c ft_strdup.c
SRCSPARTTWO = ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
		ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c \
		ft_putendl_fd.c ft_putnbr_fd.c
SRCSBONUS = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
		ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c \
		ft_lstfind.c ft_lstremove.c
SRCSPRINTF = ft_printf/ft_printf.c ft_printf/ft_putnbr_base_fd.c \
		ft_printf/ft_printf_args.c ft_printf/ft_printf_args_numbers.c \
		ft_printf/ft_printf_specifier.c ft_printf/ft_printf_args_numbers_helper.c
SRCSGETNEXTLINE = get_next_line/get_next_line_bonus.c \
		get_next_line/get_next_line_utils_bonus.c
SRCSUTILSMINISHELL = ft_strndup.c ft_strcmp_insensitive.c ft_isspace.c

OBJS = $(SRCS:.c=.o)
OBJSPARTTWO = $(SRCSPARTTWO:.c=.o)
OBJSBONUS = $(SRCSBONUS:.c=.o)
OBJSPRINTF = $(SRCSPRINTF:.c=.o)
OBJSGETNEXTLINE = $(SRCSGETNEXTLINE:.c=.o)
OBJSUTILSMINISHELL = $(SRCSUTILSMINISHELL:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(OBJSPARTTWO) $(OBJSBONUS) $(OBJSPRINTF) $(OBJSGETNEXTLINE) \
	$(OBJSUTILSMINISHELL)
	ar rc $(NAME) $(OBJS) $(OBJSPARTTWO) $(OBJSBONUS) $(OBJSPRINTF) \
		$(OBJSGETNEXTLINE) $(OBJSUTILSMINISHELL)

%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf $(OBJS) $(OBJSPARTTWO) $(OBJSBONUS) $(OBJSPRINTF) $(OBJSGETNEXTLINE) \
		$(OBJSUTILSMINISHELL)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

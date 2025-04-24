# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/27 13:09:33 by djunho            #+#    #+#              #
#    Updated: 2025/04/23 10:48:59 by bfiochi-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= minishell
SRCS = main.c read_command.c utils.c handle_input.c tokenization.c
OBJS_DIR = objs
OBJS = $(patsubst %.c,$(OBJS_DIR)/%.o,$(SRCS))
DEPENDS = $(patsubst %.c,$(OBJS_DIR)/%.d,$(SRCS))

CC = cc
CFLAGS = -Wall -Werror -Wextra -ggdb

# Check https://stackoverflow.com/questions/2214575/passing-arguments-to-make-run
# If the first argument is "run"...
ifeq (test,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif
ifeq (gdb,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

all: libft $(NAME)

bonus: all

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -Llibft -lft -lreadline -o $(NAME)

-include $(DEPENDS)

$(OBJS_DIR)/%.o:%.c Makefile | $(OBJS_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Create the object directory if it doesn't exist
$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

libft:
	make -C ./libft/ all

clean: soft_clean
	make -C ./libft/ clean

soft_clean:
	rm -rf $(OBJS) $(DEPENDS) $(OBJS_DIR)

fclean: soft_fclean
	make -C ./libft/ fclean

soft_fclean: soft_clean
	rm -rf $(NAME)

norminette:
	norminette --use-gitignore -R CheckForbiddenSourceHeader || echo "Norminette fail!!"

re: fclean all

soft_re: soft_fclean all

valgrind: all
	valgrind --track-origins=yes ./$(NAME) $(RUN_ARGS)

test: all
	./$(NAME) $(RUN_ARGS)

gdb: all
	gdb $(NAME) -ex 'run $(RUN_ARGS)'

.PHONY: all clean fclean re bonus libft test gdb

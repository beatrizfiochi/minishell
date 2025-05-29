# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: djunho <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/27 13:09:33 by djunho            #+#    #+#              #
#    Updated: 2025/04/21 19:27:48 by djunho           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= btree_test
BTREE_SRC = ../../delivery/btree/
VPATH := . $(BTREE_SRC)
SRCS = main.c test0.c test1.c test2.c test3.c test4.c \
		$(BTREE_SRC)/btree.c $(BTREE_SRC)/btree_count.c \
		$(BTREE_SRC)/btree_foreach.c
OBJS_DIR = objs
OBJS = $(patsubst %.c,$(OBJS_DIR)/%.o,$(notdir $(SRCS)))
DEPENDS = $(patsubst %.c,$(OBJS_DIR)/%.d,$(SRCS))

CC = cc
CFLAGS = -I../../delivery/btree -Wall -Werror -Wextra -ggdb

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

all: $(NAME)

bonus: all

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lreadline -o $(NAME)

-include $(DEPENDS)

$(OBJS_DIR)/%.o:%.c Makefile | $(OBJS_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Create the object directory if it doesn't exist
$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

clean:
	rm -rf $(OBJS) $(DEPENDS) $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

norminette:
	norminette --use-gitignore -R CheckForbiddenSourceHeader || echo "Norminette fail!!"

re: fclean all

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=valgrind-supression ./$(NAME) $(RUN_ARGS)

test: all
	./$(NAME) $(RUN_ARGS)

gdb: all
	gdb $(NAME) -ex 'run $(RUN_ARGS)'

.PHONY: all clean fclean re bonus test gdb

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/27 13:09:33 by djunho            #+#    #+#              #
#    Updated: 2025/07/21 08:15:42 by djunho           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= minishell
SRCS = main.c read_command.c utils.c debug.c readline.c \
	parser/tokenization.c parser/aux.c parser/aux2.c parser/aux3.c \
	parser/tree_creator.c parser/tree_creator_checks.c \
	parser/tree_creator_expand.c parser/tokens.c \
	parser/tokenization_utils2.c parser/tokenization_utils.c \
	parser/wildcards/expand_wildcards.c \
	parser/wildcards/expand_wildcards2.c \
	btree/btree.c btree/btree_count.c btree/debug.c \
	btree/btree_foreach.c \
	execute/execution.c execute/process.c execute/debug.c \
	execute/execution_operations.c execute/env_utils.c \
	execute/exec_utils.c execute/exec_utils2.c \
	execute/process_aux.c execute/process_utils.c \
	builtins/builtins.c builtins/echo/echo.c builtins/cd/cd.c \
	builtins/env/env.c builtins/export/export.c builtins/pwd/pwd.c \
	builtins/unset/unset.c \
	signals/handle_signals.c signals/heredoc_signals.c \
	builtins/exit/exit.c \
	variables/handle_variables.c variables/env_variables.c \
	variables/shell_level_var.c variables/handle_var_utils.c \
	variables/expand_variable.c variables/expand_variable_utils.c \
	variables/expand_variable2.c \
	redirect/redirect.c redirect/redirect_aux.c \
	heredoc/heredoc.c heredoc/heredoc_aux.c
OBJS_DIR = objs
OBJS = $(patsubst %.c,$(OBJS_DIR)/%.o,$(SRCS))
DEPENDS = $(patsubst %.c,$(OBJS_DIR)/%.d,$(SRCS))

CC = cc
CFLAGS += -Wall -Werror -Wextra -ggdb

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

debug: CFLAGS += -DTEST
debug: all

$(NAME): $(OBJS) libft/libft.a
	$(CC) $(CFLAGS) $(OBJS) -Llibft -lft -lreadline -o $(NAME)

-include $(DEPENDS)

# Rule for creating object files in $(OBJS_DIR)
$(OBJS_DIR)/%.o:%.c Makefile | $(OBJS_DIR) $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Create the object directory if it doesn't exist
# Create the directory for the object file if it doesn't exist
$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%:
	mkdir -p $(dir $@)

libft/libft.a: libft

libft:
	$(MAKE) -C ./libft/ all

clean: soft_clean
	$(MAKE) -C ./libft/ clean

soft_clean:
	rm -rf $(OBJS) $(DEPENDS) $(OBJS_DIR)

fclean: soft_fclean
	$(MAKE) -C ./libft/ fclean

soft_fclean: soft_clean
	rm -rf $(NAME)

norminette:
	norminette --use-gitignore -R CheckForbiddenSourceHeader || echo "Norminette fail!!"

re: fclean all

soft_re: soft_fclean all

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=valgrind-supression ./$(NAME) $(RUN_ARGS)

test: all
	./$(NAME) $(RUN_ARGS)

gdb: all
	gdb $(NAME) -ex 'run $(RUN_ARGS)'

.PHONY: all clean fclean re bonus libft test gdb debug

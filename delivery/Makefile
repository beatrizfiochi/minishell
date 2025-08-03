# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/27 13:09:33 by djunho            #+#    #+#              #
#    Updated: 2025/08/03 13:20:33 by bfiochi-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
BONUS_NAME = minishell_bonus

SRCS_DIR = ./mandatory
BONUS_DIR = ./bonus

SRCS = $(SRCS_DIR)/main.c \
	$(SRCS_DIR)/read_command.c \
	$(SRCS_DIR)/utils.c \
	$(SRCS_DIR)/debug.c \
	$(SRCS_DIR)/debug2.c \
	$(SRCS_DIR)/readline.c \
	$(SRCS_DIR)/shell_aux.c \
	$(SRCS_DIR)/parser/tokenization.c \
	$(SRCS_DIR)/parser/aux.c \
	$(SRCS_DIR)/parser/aux2.c \
	$(SRCS_DIR)/parser/tree_creator.c \
	$(SRCS_DIR)/parser/tree_creator_checks.c \
	$(SRCS_DIR)/parser/tree_creator_expand.c \
	$(SRCS_DIR)/parser/tokens.c \
	$(SRCS_DIR)/parser/tokens2.c \
	$(SRCS_DIR)/parser/tokenization_utils2.c \
	$(SRCS_DIR)/parser/tokenization_utils.c \
	$(SRCS_DIR)/parser/tokenization_utils3.c \
	$(SRCS_DIR)/btree/btree.c \
	$(SRCS_DIR)/btree/btree_count.c \
	$(SRCS_DIR)/btree/debug.c \
	$(SRCS_DIR)/btree/btree_foreach.c \
	$(SRCS_DIR)/execute/execution.c \
	$(SRCS_DIR)/execute/process.c \
	$(SRCS_DIR)/execute/debug.c \
	$(SRCS_DIR)/execute/execution_operations.c \
	$(SRCS_DIR)/execute/env_utils.c \
	$(SRCS_DIR)/execute/exec_utils.c \
	$(SRCS_DIR)/execute/exec_utils2.c \
	$(SRCS_DIR)/execute/process_aux.c \
	$(SRCS_DIR)/execute/process_utils.c \
	$(SRCS_DIR)/execute/parenthesis.c \
	$(SRCS_DIR)/execute/envp_utils.c \
	$(SRCS_DIR)/builtins/builtins.c \
	$(SRCS_DIR)/builtins/echo/echo.c \
	$(SRCS_DIR)/builtins/cd/cd.c \
	$(SRCS_DIR)/builtins/env/env.c \
	$(SRCS_DIR)/builtins/export/export.c \
	$(SRCS_DIR)/builtins/pwd/pwd.c \
	$(SRCS_DIR)/builtins/unset/unset.c \
	$(SRCS_DIR)/signals/handle_signals.c \
	$(SRCS_DIR)/signals/heredoc_signals.c \
	$(SRCS_DIR)/signals/handle_signals_utils.c \
	$(SRCS_DIR)/builtins/exit/exit.c \
	$(SRCS_DIR)/variables/handle_variables.c \
	$(SRCS_DIR)/variables/env_variables.c \
	$(SRCS_DIR)/variables/shell_level_var.c \
	$(SRCS_DIR)/variables/handle_var_utils.c \
	$(SRCS_DIR)/variables/expand_variable.c \
	$(SRCS_DIR)/variables/expand_variable_utils.c \
	$(SRCS_DIR)/variables/expand_variable2.c \
	$(SRCS_DIR)/variables/expand_variables3.c \
	$(SRCS_DIR)/redirect/redirect.c \
	$(SRCS_DIR)/redirect/redirect_aux.c \
	$(SRCS_DIR)/redirect/redirect_aux2.c \
	$(SRCS_DIR)/redirect/pipe.c \
	$(SRCS_DIR)/heredoc/heredoc.c \
	$(SRCS_DIR)/heredoc/heredoc_aux.c

SRCS_BONUS = $(BONUS_DIR)/main_bonus.c \
	$(BONUS_DIR)/read_command_bonus.c \
	$(BONUS_DIR)/utils_bonus.c \
	$(BONUS_DIR)/debug_bonus.c \
	$(BONUS_DIR)/debug2_bonus.c \
	$(BONUS_DIR)/readline_bonus.c \
	$(BONUS_DIR)/shell_aux_bonus.c \
	$(BONUS_DIR)/parser/tokenization_bonus.c \
	$(BONUS_DIR)/parser/aux_bonus.c \
	$(BONUS_DIR)/parser/aux2_bonus.c \
	$(BONUS_DIR)/parser/tree_creator_bonus.c \
	$(BONUS_DIR)/parser/tree_creator_checks_bonus.c \
	$(BONUS_DIR)/parser/tree_creator_expand_bonus.c \
	$(BONUS_DIR)/parser/tokens_bonus.c \
	$(BONUS_DIR)/parser/tokens2_bonus.c \
	$(BONUS_DIR)/parser/tokenization_utils2_bonus.c \
	$(BONUS_DIR)/parser/tokenization_utils_bonus.c \
	$(BONUS_DIR)/parser/tokenization_utils3_bonus.c \
	$(BONUS_DIR)/parser/wildcards/expand_wildcards_bonus.c \
	$(BONUS_DIR)/parser/wildcards/expand_wildcards2_bonus.c \
	$(BONUS_DIR)/parser/wildcards/expand_wildcards3_bonus.c \
	$(BONUS_DIR)/btree/btree_bonus.c \
	$(BONUS_DIR)/btree/btree_count_bonus.c \
	$(BONUS_DIR)/btree/debug_bonus.c \
	$(BONUS_DIR)/btree/btree_foreach_bonus.c \
	$(BONUS_DIR)/execute/execution_bonus.c \
	$(BONUS_DIR)/execute/process_bonus.c \
	$(BONUS_DIR)/execute/debug_bonus.c \
	$(BONUS_DIR)/execute/execution_operations_bonus.c \
	$(BONUS_DIR)/execute/env_utils_bonus.c \
	$(BONUS_DIR)/execute/exec_utils_bonus.c \
	$(BONUS_DIR)/execute/exec_utils2_bonus.c \
	$(BONUS_DIR)/execute/process_aux_bonus.c \
	$(BONUS_DIR)/execute/process_utils_bonus.c \
	$(BONUS_DIR)/execute/parenthesis_bonus.c \
	$(BONUS_DIR)/execute/envp_utils_bonus.c \
	$(BONUS_DIR)/builtins/builtins_bonus.c \
	$(BONUS_DIR)/builtins/echo/echo_bonus.c \
	$(BONUS_DIR)/builtins/cd/cd_bonus.c \
	$(BONUS_DIR)/builtins/env/env_bonus.c \
	$(BONUS_DIR)/builtins/export/export_bonus.c \
	$(BONUS_DIR)/builtins/pwd/pwd_bonus.c \
	$(BONUS_DIR)/builtins/unset/unset_bonus.c \
	$(BONUS_DIR)/signals/handle_signals_bonus.c \
	$(BONUS_DIR)/signals/heredoc_signals_bonus.c \
	$(BONUS_DIR)/signals/handle_signals_utils_bonus.c \
	$(BONUS_DIR)/builtins/exit/exit_bonus.c \
	$(BONUS_DIR)/variables/handle_variables_bonus.c \
	$(BONUS_DIR)/variables/env_variables_bonus.c \
	$(BONUS_DIR)/variables/shell_level_var_bonus.c \
	$(BONUS_DIR)/variables/handle_var_utils_bonus.c \
	$(BONUS_DIR)/variables/expand_variable_bonus.c \
	$(BONUS_DIR)/variables/expand_variable_utils_bonus.c \
	$(BONUS_DIR)/variables/expand_variable2_bonus.c \
	$(BONUS_DIR)/variables/expand_variables3_bonus.c \
	$(BONUS_DIR)/redirect/redirect_bonus.c \
	$(BONUS_DIR)/redirect/redirect_aux_bonus.c \
	$(BONUS_DIR)/redirect/redirect_aux2_bonus.c \
	$(BONUS_DIR)/redirect/pipe_bonus.c \
	$(BONUS_DIR)/heredoc/heredoc_bonus.c \
	$(BONUS_DIR)/heredoc/heredoc_aux_bonus.c

OBJS_DIR = objs
OBJS = $(patsubst %.c,$(OBJS_DIR)/%.o,$(SRCS))
DEPENDS = $(patsubst %.c,$(OBJS_DIR)/%.d,$(SRCS))

OBJS_DIR_BONUS = objs_bonus
OBJS_BONUS = $(patsubst %.c,$(OBJS_DIR)/%.o,$(SRCS_BONUS))
DEPENDS_BONUS = $(patsubst %.c,$(OBJS_DIR)/%.d,$(SRCS_BONUS))

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

bonus: libft $(BONUS_NAME)

debug: CFLAGS += -DTEST
debug: all

$(NAME): $(OBJS) libft/libft.a
	$(CC) $(CFLAGS) $(OBJS) -I. -Llibft -lft -lreadline -o $(NAME)

$(BONUS_NAME): $(OBJS_BONUS) libft/libft.a
	$(CC) $(CFLAGS) $(OBJS_BONUS) -I. -Llibft -lft -lreadline -o $(BONUS_NAME)

-include $(DEPENDS) $(DEPENDS_BONUS)

# Rule for creating object files in $(OBJS_DIR)
$(OBJS_DIR)/%.o:%.c Makefile | $(OBJS_DIR) $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(OBJS_DIR_BONUS)/%.o:%.c Makefile | $(OBJS_DIR_BONUS) $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Create the object directory if it doesn't exist
# Create the directory for the object file if it doesn't exist
$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%:
	mkdir -p $(dir $@)

$(OBJS_DIR_BONUS):
	mkdir -p $(OBJS_DIR_BONUS)

$(OBJS_DIR_BONUS)/%:
	mkdir -p $(dir $@)

libft/libft.a: libft

libft:
	$(MAKE) -C ./libft/ all

clean: soft_clean
	$(MAKE) -C ./libft/ clean

soft_clean:
	rm -rf $(OBJS) $(OBJS_BONUS) $(DEPENDS) $(DEPENDS_BONUS) \
			$(OBJS_DIR) $(OBJS_DIR_BONUS)

soft_fclean: soft_clean
	rm -rf $(NAME)

soft_fclean_bonus: soft_clean
	rm -rf $(BONUS_NAME)

fclean: soft_fclean soft_fclean_bonus
	$(MAKE) -C ./libft/ fclean

norminette:
	norminette --use-gitignore -R CheckForbiddenSourceHeader || echo "Norminette fail!!"

re: fclean all

re_bonus: fclean bonus

soft_re: soft_fclean all

soft_re_bonus: soft_fclean_bonus bonus

valgrind: all
	valgrind --trace-children=yes --child-silent-after-fork=no --track-origins=yes \
		--leak-check=full --show-leak-kinds=all --suppressions=../valgrind-suppression \
		./$(NAME) $(RUN_ARGS)

valgrind_bonus: all
	valgrind --trace-children=yes --child-silent-after-fork=no --track-origins=yes \
		--leak-check=full --show-leak-kinds=all --suppressions=../valgrind-suppression \
		./$(BONUS_NAME) $(RUN_ARGS)

.PHONY: all clean fclean re re_bonus bonus libft debug

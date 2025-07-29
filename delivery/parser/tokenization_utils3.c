/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 23:17:46 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/29 23:47:20 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "aux.h"
#include "../cmd.h"
#include "../redirect/redirect_aux.h"

// Basic op: && || |
static bool	is_basic_op(t_node_op op)
{
	return ((op == OP_AND) || (op == OP_OR) || (op == OP_PIPE));
}
/*
check_token - Validates the sequence of two operator tokens in a list.
This function checks whether a pair of consecutive tokens (`prev` and `curr`)
in a token list form a syntactically valid sequence in the context of a shell
command parser. It helps ensure that operators and command structures appear
in correct order and combination.
@param prev A pointer to the previous token in the list.
@param curr A pointer to the current token in the list.
@return EXIT_FAILURE if the token combination is invalid,
        EXIT_SUCCESS if the combination is syntactically valid.
Invalid cases include:
- An opening parenthesis immediately following a command or redirection target.
- Two basic operators (e.g., `&&`, `||`, `|`) in a row.
- A basic operator following an opening parenthesis or a redirection target.
- A closing parenthesis following an opening parenthesis, a basic operator, or a redirection target.
- Two redirection operators in a row.
- A command following a closing parenthesis.
 */
int	check_token(t_list *prev, t_list *curr)
{
	t_node_op	curr_op;
	t_node_op	prev_op;

	curr_op = op_list(curr);
	prev_op = op_list(prev);
	if ((curr_op == OP_PAREN_OPEN) && ((is_redirect_file_op(prev_op))
		|| ((prev_op == OP_CMD))))
		return (EXIT_FAILURE);
	else if ((is_basic_op(curr_op)) && ((is_basic_op(prev_op))
		|| (prev_op == OP_PAREN_OPEN) || (is_redirect_file_op(prev_op))))
		return (EXIT_FAILURE);
	else if ((curr_op == OP_PAREN_CLOSE) && ((is_basic_op(prev_op))
		|| (prev_op == OP_PAREN_OPEN) || (is_redirect_file_op(prev_op))))
		return (EXIT_FAILURE);
	else if ((is_redirect_file_op(curr_op)) && (is_redirect_file_op(prev_op)))
		return (EXIT_FAILURE);
	else if ((curr_op == OP_CMD) && (prev_op == OP_PAREN_CLOSE))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

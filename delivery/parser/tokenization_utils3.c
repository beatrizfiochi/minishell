/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 23:17:46 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/30 15:18:23 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "aux.h"
#include "../cmd.h"
#include "../redirect/redirect_aux.h"

// Basic op: && || |
bool	is_basic_op(t_node_op op)
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
- A closing parenthesis following an opening parenthesis,
		a basic operator, or a redirection target.
- Two redirection operators in a row.
- A command following a closing parenthesis.
 */
int	check_token(t_list *prev, t_list *curr)
{
	t_node_op	curr_op;
	t_node_op	prev_op;

	curr_op = op_list(curr);
	prev_op = op_list(prev);
	if ((curr_op == OP_INVALID) && (((is_basic_op(prev_op))
				|| (is_redirect_file_op(prev_op))
				|| (prev_op == OP_PAREN_OPEN))))
		return (EXIT_FAILURE);
	else if ((curr_op == OP_PAREN_OPEN) && ((is_redirect_file_op(prev_op))
			|| ((prev_op == OP_CMD))))
		return (EXIT_FAILURE);
	else if ((is_basic_op(curr_op)) && ((is_basic_op(prev_op))
			|| (prev_op == OP_PAREN_OPEN) || (is_redirect_file_op(prev_op))
			|| (prev_op == OP_INVALID)))
		return (EXIT_FAILURE);
	else if ((curr_op == OP_PAREN_CLOSE) && ((is_basic_op(prev_op))
			|| (prev_op == OP_PAREN_OPEN) || (is_redirect_file_op(prev_op))
			|| (prev_op == OP_INVALID)))
		return (EXIT_FAILURE);
	else if ((is_redirect_file_op(curr_op)) && (is_redirect_file_op(prev_op)))
		return (EXIT_FAILURE);
	else if ((curr_op == OP_CMD) && (prev_op == OP_PAREN_CLOSE))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/*
 is_invalid_cmd_after_paren - Checks if the token pattern matches:
     redirect preceded by closing parenthesis, followed by two command tokens.

 @token: Current token in the list (assumed to be a redirection operator)

 Return: true if pattern is matched, false otherwise.
*/
static bool	is_invalid_cmd_after_paren(t_list *token)
{
	t_node_op	next;
	t_node_op	after_next;

	if (token->next != NULL)
	{
		next = op_list(token->next);
		if ((next == OP_CMD) && (token->next->next != NULL))
		{
			after_next = op_list(token->next->next);
			if (after_next == OP_CMD)
				return (true);
		}
	}
	return (false);
}

/*
 check_special_with_reddir - Checks for a specific invalid pattern
                             involving redirection and command tokens.

 This function traverses a linked list of tokens and searches for a pattern
 where a redirection operator (e.g., >, >>, <) is immediately preceded by a
 closing parenthesis (OP_PAREN_CLOSE), and followed by two consecutive
 command tokens (OP_CMD). If such a pattern is found, it indicates a syntax
 error and the function returns EXIT_FAILURE.
 @token: A pointer to the head of a linked list of tokens (t_list *)
 Return: EXIT_FAILURE if the invalid pattern is found,
         otherwise EXIT_SUCCESS.
*/
int	check_special_with_reddir(t_list *token)
{
	t_list		*head;
	t_node_op	curr;
	t_node_op	prev;

	head = token;
	while (token != NULL)
	{
		curr = op_list(token);
		if (is_redirect_file_op(curr))
		{
			prev = op_list(prev_list_item(head, token));
			if ((prev == OP_PAREN_CLOSE) && (is_invalid_cmd_after_paren(token)))
				return (EXIT_FAILURE);
		}
		token = token->next;
	}
	return (EXIT_SUCCESS);
}

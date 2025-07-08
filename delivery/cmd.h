/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:23:29 by djunho            #+#    #+#             */
/*   Updated: 2025/07/06 19:57:50 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_H
# define CMD_H

# include "libft/libft.h"

typedef enum e_node_op
{
	OP_INVALID = 0,
	OP_AND,
	OP_OR,
	OP_PIPE,
	OP_VAR_ASSIGN,
	OP_RD_INPUT,
	OP_RD_OUTPUT,
	OP_HEREDOC,
	OP_APPEND_RD_OUTPUT,
	OP_CMD,
	OP_PAREN_OPEN,
	OP_PAREN_CLOSE,
	OP_SIZE_DO_NOT_USE,
}	t_node_op;

typedef struct s_cmd
{
	t_list	*tokens;		// cmd followed by args
	bool	is_builtin;		// Updated once executed. False by default
	bool	finished;		// Indicated if the pid was already waited for.
							//	False by default
	struct s_cmd_redirect
	{
		int		fd;
	} redir;
}	t_cmd;

typedef struct s_content_node
{
	t_node_op	op;
	union
	{
		t_cmd		cmd;
	};
}	t_content_node;

#endif // CMD_H

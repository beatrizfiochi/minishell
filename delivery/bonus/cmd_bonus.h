/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:23:29 by djunho            #+#    #+#             */
/*   Updated: 2025/07/27 18:15:31 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_H
# define CMD_H

# include "../libft/libft.h"
# include "btree/btree_bonus.h"

typedef enum e_node_op
{
	OP_INVALID = 0,
	OP_AND,
	OP_OR,
	OP_PIPE,
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
	bool	is_parentheses;	// True if the operator or command is a begining
							//	of a parentheses. False by default
	int		pipe_fd;		// The file descriptor that is forward to the other
							//  cmd. This field will only be used by OP_PIPE
							//  operator
	struct s_cmd_redirect
	{
		int		fd_in;		// File descriptor for input redirection (placed 
							//   on possible command and on operator)
		int		fd_out;		// File descriptor for output redirection
							//   on possible command and on operator)
	} redir;
}	t_cmd;

typedef struct s_content_node
{
	t_node_op	op;
	t_cmd		cmd;
}	t_content_node;

t_content_node	*node_cnt(const t_btnode *node);

#endif // CMD_H

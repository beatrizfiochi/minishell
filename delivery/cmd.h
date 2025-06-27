/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:23:29 by djunho            #+#    #+#             */
/*   Updated: 2025/06/23 15:22:39 by djunho           ###   ########.fr       */
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
	OP_PARENTESIS_OPEN,
	OP_PARENTESIS_CLOSE,
	OP_SIZE_DO_NOT_USE,
}	t_node_op;

typedef struct s_cmd
{
	t_list	*tokens;		// cmd followed by args
	bool	is_builtin;		// Updated once executed. False by default
	bool	finished;		// Indicated if the pid was already waited for. False by default
}	t_cmd;

typedef struct s_pipe
{
	int		pipe[2];			// pipe file descriptors
	int		carry_over_fd;		// file descriptor to carry over the pipe
	bool	is_last_pipe;
}	t_pipe;

typedef struct s_content_node
{
	t_node_op	op;
	union
	{
		t_cmd		cmd;
		t_pipe		pipe;
	};
}	t_content_node;

#endif // CMD_H

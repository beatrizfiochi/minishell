/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:23:29 by djunho            #+#    #+#             */
/*   Updated: 2025/05/30 16:19:36 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_H
# define CMD_H

# include "libft/libft.h"
# include "parser/parser.h"

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
	OP_SIZE_DO_NOT_USE,
}	t_node_op;

typedef struct s_cmd
{
	t_list	*tokens;		// cmd followed by args
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

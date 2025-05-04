/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:23:29 by djunho            #+#    #+#             */
/*   Updated: 2025/05/04 13:55:10 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CMD_H
# define CMD_H

# include "libft/libft.h"

typedef enum e_node_op
{
	OP_INVALID,
	OP_AND,
	OP_OR,
	OP_PIPE,
	OP_CMD,
}	t_node_op;

typedef struct s_cmd
{
	t_list	*tokens;		// cmd followed by args
}	t_cmd;

typedef struct s_content_node
{
	t_node_op	op;
	t_cmd		cmd;
}	t_content_node;

#endif // CMD_H

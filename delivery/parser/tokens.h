/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 08:08:11 by djunho            #+#    #+#             */
/*   Updated: 2025/07/22 23:04:18 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
# define TOKENS_H

# include "../libft/libft.h"
# include "aux.h"

struct s_split_token_list
{
	t_list	*left;
	t_list	*op;
	t_list	*right;
	t_list	*remain;
};

int		split_tokens(t_list **tokens, struct s_split_token_list *split,
			bool has_left, enum e_expand_type *expand);

bool	split_token_list(struct s_split_token_list *split,
			enum e_expand_type expand_type);

void	*abort_tree_lst(t_btnode *tree, struct s_split_token_list *split);

#endif // TOKENS_H

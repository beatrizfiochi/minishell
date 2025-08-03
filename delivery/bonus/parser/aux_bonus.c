/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:09:35 by djunho            #+#    #+#             */
/*   Updated: 2025/07/22 09:42:14 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>				// STDERR_FILENO
#include "../libft/libft.h"
#include "../cmd.h"
#include "aux.h"
#include "tokens.h"	// struct s_split_token_list

t_list	*prev_list_item(t_list *first_node, t_list *current_node)
{
	if (first_node == NULL || first_node == current_node)
		return (NULL);
	if (current_node == NULL)
		return (NULL);
	while (first_node->next != NULL && first_node->next != current_node)
		first_node = first_node->next;
	return (first_node);
}

void	free_btree_content(void *_content)
{
	t_content_node	*content;

	content = (t_content_node *)_content;
	if (content->cmd.tokens != NULL)
		ft_lstclear(&content->cmd.tokens, free);
	free(content);
}

bool	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

void	*abort_tree_lst(t_btnode *tree, struct s_split_token_list *split)
{
	if (split != NULL)
	{
		if (split->right != NULL)
			ft_fprintf(STDERR_FILENO, "syntax error near unexpected token `%s'"
				"\n", (char *)split->right->content);
		else if (split->op != NULL)
			ft_fprintf(STDERR_FILENO, "syntax error near unexpected token `%s'"
				"\n", (char *)split->op->content);
		else if (split->left != NULL)
			ft_fprintf(STDERR_FILENO, "syntax error near unexpected token `%s'"
				"\n", (char *)split->left->content);
	}
	if (tree != NULL)
		btree_clear(&tree, free_btree_content);
	if (split != NULL)
	{
		ft_lstclear(&split->left, free);
		ft_lstclear(&split->op, free);
		ft_lstclear(&split->right, free);
		ft_lstclear(&split->remain, free);
	}
	return (NULL);
}

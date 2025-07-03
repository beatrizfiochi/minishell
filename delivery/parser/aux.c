/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:09:35 by djunho            #+#    #+#             */
/*   Updated: 2025/06/23 18:18:53 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../libft/libft.h"
#include "../cmd.h"
#include "aux.h"

t_list	*prev_list_item(t_list *first_node, t_list *current_node)
{
	if (first_node == NULL || first_node == current_node)
		return (NULL);
	while (first_node->next != NULL && first_node->next != current_node)
		first_node = first_node->next;
	return (first_node);
}

void	free_btree_content(void *_content)
{
	t_content_node	*content;

	content = (t_content_node *)_content;
	if (content->op == OP_CMD)
	{
		if (content->cmd.tokens != NULL)
			ft_lstclear(&content->cmd.tokens, free);
	}
	free(content);
}

void	*abort_tree_lst(t_btnode *tree, t_list **token_list,
			const char *msg)
{
	if (msg != NULL)
		printf("%s", msg);
	if (tree != NULL)
		btree_clear(&tree, free_btree_content);
	if (token_list != NULL && *token_list != NULL)
		ft_lstclear(token_list, free);
	return (NULL);
}

bool	split_token_list(struct s_split_token_list *split, bool need_left,
			enum e_expand_type expand_type)
{
	t_list	*aux;

	while (1)
	{
		if (split->left == NULL)
			break ;
		split->op = search_op(split->left, expand_type);
		if (split->op == NULL)
			break ;
		aux = prev_list_item(split->left, split->op);
		if ((split->op->next == NULL) || (need_left && (aux == NULL)))
			break ;
		split->right = split->op->next;
		split->remain = search_op(split->right, expand_type);
		if (split->remain == split->right)
			break ;
		if (aux != NULL)
			aux->next = NULL;
		split->op->next = NULL;
		if (split->remain != NULL)
			prev_list_item(split->right, split->remain)->next = NULL;
		return (true);
	}
	abort_tree_lst(NULL, &(split->left), "Error: spliting tokens from list\n");
	return (false);
}

bool	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:11:46 by djunho            #+#    #+#             */
/*   Updated: 2025/05/18 20:30:13 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../cmd.h"
#include "../btree/btree.h"

#ifdef TEST

//Attention: the fflush function is forbitten. But it is not used on targhet all
void	debug_btree_print(t_btnode *node)
{
	t_list	*cmd;

	if (node == NULL)
		return ;
	if (node->content != NULL)
	{
		printf("Exec: Operator> %d\n", ((t_content_node *)node->content)->op);
		printf("Exec: Command > cmd:");
		cmd = ((t_content_node *)node->content)->cmd.tokens;
		while (cmd != NULL)
		{
			printf(" %s", (char *)cmd->content);
			cmd = cmd->next;
		}
		printf("\n");
	}
	if (node->left != NULL)
		debug_btree_print(node->left);
	if (node->right != NULL)
		debug_btree_print(node->right);
	fflush(stdout);
}
#endif

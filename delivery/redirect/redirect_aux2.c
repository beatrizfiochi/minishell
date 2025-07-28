/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_aux2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 08:14:24 by djunho            #+#    #+#             */
/*   Updated: 2025/07/28 21:57:31 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>                    // close
#include "../minishell.h"
#include "../parser/parser.h"
#include "../execute/execution.h"

void	close_redirects(t_shell *shell)
{
	shell->is_running_redirect = false;
	shell->is_last_redirect = false;
	close_possible_pipe(shell);
}

char	*get_redir_filename(t_shell *shell, t_btnode *file_node)
{
	t_list	*token;
	int		size;

	token = ((t_content_node *)(file_node->content))->cmd.tokens;
	expand_variable_token(&token, shell->variable_list, shell);
	expand_wildcards_token(&token);
	clean_token_quotes(token);
	((t_content_node *)(file_node->content))->cmd.tokens = token;
	size = ft_lstsize(token);
	if ((size != 1) || (((char *)token->content)[0] == '\0'))
	{
		ft_fprintf(STDERR_FILENO, "Ambiguous redirect\n");
		return (NULL);
	}
	return (((t_content_node *)(file_node->content))->cmd.tokens->content);
}

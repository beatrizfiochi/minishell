/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_aux2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 08:14:24 by djunho            #+#    #+#             */
/*   Updated: 2025/07/26 00:25:30 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>			// close
#include "../minishell.h"
#include "../parser/parser.h"

void	close_redirects(t_shell *shell)
{
	shell->is_running_redirect = false;
	shell->is_last_redirect = false;
	close(shell->pipe.pipe[0]);
	close(shell->pipe.pipe[1]);
	shell->pipe.pipe[0] = -1;
	shell->pipe.pipe[1] = -1;
}

char	*get_redir_filename(t_shell *shell, t_btnode *file_node)
{
	t_list	*token;

	token = ((t_content_node *)(file_node->content))->cmd.tokens;
	expand_variable_token(&token, shell->variable_list, shell);
	clean_token_quotes(token);
	if (ft_lstsize(((t_content_node *)(file_node->content))->cmd.tokens) != 1)
	{
		ft_fprintf(STDERR_FILENO, "Ambiguous redirect\n");
		return (NULL);
	}
	return (((t_content_node *)(file_node->content))->cmd.tokens->content);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 21:20:02 by djunho            #+#    #+#             */
/*   Updated: 2025/07/11 21:40:54 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "../libft/libft.h"
#include "../parser/aux.h"			// is_quote()
#include "../parser/parser.h"		// clean_string_quotes()

char	*get_heredoc_file_name(void)
{
	static int	heredoc_counter = 0;
	char		*file_name;
	char		*id;

	id = ft_itoa(heredoc_counter++);
	if (id == NULL)
		return (NULL);
	file_name = ft_strjoin("/tmp/heredoc_", id);
	free(id);
	return (file_name);
}

bool	should_expand(char **eof)
{
	if (is_quote((*eof)[0]) && clean_string_quotes(eof) && !is_quote((*eof)[0]))
	{
		return (false);
	}
	return (true);
}

// The heredoc does not expand the tmp variable list
void	expand_line(t_shell *shell, bool should_expand, char **line)
{
	if (!should_expand)
		return ;
	search_and_expand_string(line, shell->variable_list, shell);
}

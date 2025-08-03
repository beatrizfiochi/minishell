/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 21:20:02 by djunho            #+#    #+#             */
/*   Updated: 2025/07/31 21:44:42 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "../../libft/libft.h"
#include "../parser/aux_bonus.h"			// is_quote()
#include "../parser/parser_bonus.h"		// clean_string_quotes()
#include "../variables/expand_variables_bonus.h"

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

static bool	has_quotes(char *eof)
{
	while (*eof != '\0')
	{
		if (*eof == '"' || *eof == '\'')
		{
			mark_quotes(eof);
			return (true);
		}
		eof++;
	}
	return (false);
}

bool	should_expand(char **eof)
{
	bool	quotes;

	quotes = has_quotes(*eof);
	if ((quotes == true) && clean_string_quotes(eof))
		return (false);
	return (true);
}

// The heredoc does not expand the tmp variable list
void	expand_line(t_shell *shell, bool should_expand, char **line)
{
	if (!should_expand)
		return ;
	expand_variable_string(line, shell->variable_list, shell);
}

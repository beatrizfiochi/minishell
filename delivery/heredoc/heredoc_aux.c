/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 21:20:02 by djunho            #+#    #+#             */
/*   Updated: 2025/07/27 16:30:52 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "../libft/libft.h"
#include "../parser/aux.h"			// is_quote()
#include "../parser/parser.h"		// clean_string_quotes()
#include "../variables/expand_variables.h"

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
	if (((*eof)[0] == '"') || ((*eof)[0] == '\''))
			mark_quotes(*eof);
	if (((*eof)[0] == QUOTE_MARK) && clean_string_quotes(eof)
		&& ((*eof)[0] != QUOTE_MARK))
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

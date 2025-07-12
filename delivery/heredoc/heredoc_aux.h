/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_aux.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 21:41:09 by djunho            #+#    #+#             */
/*   Updated: 2025/07/11 21:45:09 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef HEREDOC_AUX_H
# define HEREDOC_AUX_H

# include <stdbool.h>
# include "../minishell.h"

char	*get_heredoc_file_name(void);
bool	should_expand(char **eof);
void	expand_line(t_shell *shell, bool should_expand, char **line);

#endif // HEREDOC_AUX_H

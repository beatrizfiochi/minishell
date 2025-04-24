/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:37:40 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/04/24 14:30:20 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define PROMPT "My shell > "

# include "libft/libft.h"

typedef struct s_tokens
{
	char			*content;
	struct s_tokens	*next;
}	t_tokens;

int		run_minishell(void);

void	printf_error(const char *error);

char	*clean_string(char *str);           //APAGAR

t_tokens	*tokenization(char *line);

char	*ft_strndup(const char *s, int len);

void	free_tokens(t_tokens *list);

#endif

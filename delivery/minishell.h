/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:37:40 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/04/27 16:35:13 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"

int		run_minishell(void);

void	printf_error(const char *error);

//TODO: APAGAR
char	*clean_string(char *str);

t_list	*tokenization(char *line);

bool	search_op(char *line);

char	*go_next_char(char *line, char c);

#endif

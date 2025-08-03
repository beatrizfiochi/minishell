/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:31:19 by djunho            #+#    #+#             */
/*   Updated: 2025/07/10 23:08:48 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef HEREDOC_H
# define HEREDOC_H

# include "../btree/btree.h"
# include "../minishell.h"

int	check_tree_for_heredoc(t_shell *shell, t_btnode *node);

#endif // HEREDOC_H

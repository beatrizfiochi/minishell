/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 11:30:30 by djunho            #+#    #+#             */
/*   Updated: 2025/07/30 23:21:58 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_H
# define REDIRECT_H

# include "../minishell_bonus.h"

int		process_pipe(t_shell *shell, t_btnode *node);
int		process_redirect(t_shell *shell, int ret, t_btnode *node,
			bool *should_continue);

int		prepare_pipe(t_shell *shell, const t_btnode *node);
int		prepare_redirect_out(t_shell *shell, t_btnode *op_node);
int		prepare_redirect_in(t_shell *shell, t_btnode *op_node);
int		prepare_redirect(t_shell *shell, t_btnode *node, t_cmd *cmd);

void	close_redirects(t_shell *shell);

#endif // REDIRECT_H

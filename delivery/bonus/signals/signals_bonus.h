/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 21:18:36 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/31 20:48:36 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_BONUS_H
# define SIGNALS_BONUS_H

# include "../minishell_bonus.h"

extern int	g_signal;

void		handle_sigint(int signal);
void		init_signals(void);
void		heredoc_signals(void);
void		subprocess_signals(void);
void		ignore_signals(void);
void		handle_signal_output(int ret);
void		heredoc_ignore_signals(void);
void		handle_ctrl_c(t_shell *shell);

#endif

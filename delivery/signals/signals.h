/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 21:18:36 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/30 20:08:34 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "../minishell.h"

extern int	g_signal;

void		handle_sigint(int signal);
void		init_signals(void);
void		reset_signals(void);
void		ignore_signals(void);
void		handle_signal_output(t_shell *shell);

#endif

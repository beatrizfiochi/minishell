/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:46:18 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/31 20:57:08 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "signals.h"

void	handle_ctrl_c(t_shell *shell)
{
	if (g_signal != 0)
		shell->last_exit_status = (EXIT_SIGNAL_BASE + g_signal);
	g_signal = 0;
}

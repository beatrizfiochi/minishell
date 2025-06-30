/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:29:02 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/30 20:32:52 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../signals/signals.h"
#include "execution.h"

int	update_exit_status_and_print(t_shell *shell, int wstatus)
{
	if (WIFSIGNALED(wstatus))
		shell->last_exit_status = WTERMSIG(wstatus);
	else
		shell->last_exit_status = get_fork_return(wstatus);
	handle_signal_output(shell);
	return (get_fork_return(wstatus));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_aux2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 08:14:24 by djunho            #+#    #+#             */
/*   Updated: 2025/07/23 08:15:20 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../minishell.h"
#include <unistd.h>			// close

void	close_redirects(t_shell *shell)
{
	shell->is_running_redirect = false;
	shell->is_last_redirect = false;
	close(shell->pipe.pipe[0]);
	close(shell->pipe.pipe[1]);
	shell->pipe.pipe[0] = -1;
	shell->pipe.pipe[1] = -1;
}

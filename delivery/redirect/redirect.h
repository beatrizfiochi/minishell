/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 11:30:30 by djunho            #+#    #+#             */
/*   Updated: 2025/07/06 17:29:16 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_H
# define REDIRECT_H

# include "../minishell.h"

int	prepare_redirect_out(t_shell *shell, t_btnode *op_node);
int	prepare_redirect_in(t_shell *shell, t_btnode *op_node);

#endif // REDIRECT_H

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:16:32 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/05/30 14:47:15 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARIABLES_H
# define VARIABLES_H

# include "../libft/libft.h"
# include "../btree/btree.h"
# include "../minishell.h"

t_list		*create_var_node(char *name, char *value);

int			process_var_assign(t_btnode *node, t_shell *shell);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:10:01 by djunho            #+#    #+#             */
/*   Updated: 2025/05/05 18:10:54 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef AUX_H
# define AUX_H

# include "../libft/libft.h"

// Find the previous item of the current_node
t_list	*prev_list_item(t_list *first_node, t_list *current_node);

#endif // AUX_H

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_level_var.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:45:35 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/26 17:53:40 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../parser/parser.h"

static char	*find_var_value(char *var_name, t_list *var_list)
{
	t_content_var	*var;

	var = (t_content_var *)var_list->content;
	while (var_list != NULL)
	{
		if () procurar/ comparar o var_name com o name da var_list -> a ideia e retornar o value 
		var_list = var_list->next;
	}
}

void	update_shell_level(t_list *var_list)
{
encontrar o valor -> find_var_value
converter pra int
acrescentar +1
converter para char novamente
}

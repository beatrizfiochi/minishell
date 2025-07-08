/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:43:42 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/08 16:11:06 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../../parser/parser.h"
#include "../../libft/libft.h"
#include "../../execute/execution.h"
#include "../../variables/variables.h"

static void	print_export(t_shell *shell)
{
	t_list			*var_list;
	t_content_var	*var_content;

	var_list = shell->variable_list;
	while (var_list != NULL)
	{
		var_content = (t_content_var *)var_list->content;
		if (var_content->is_exported == true)
			ft_printf("declare -x %s=\"%s\"\n",
					var_content->var_name, var_content->var_value);
		var_list = var_list->next;
	}
}

int	export(int argc, char *argv[], t_shell *shell)
{
	int				i;
	char			*var;
	t_list			*tmp_list;
	t_content_var	*var_content;

	i = 1;
	if (i == argc)
		print_export(shell);
	else if (i < argc)
	{
		while (i < argc)
		{
			var = malloc(sizeof (char) * (ft_strlen(argv[i])) + 1);
			if (var == NULL)
				return (1);
			ft_strlcpy(var, argv[i], (ft_strlen(argv[i]) + 1));
			tmp_list = shell->variable_list;
			while (tmp_list != NULL)
			{
				var_content = (t_content_var *)tmp_list->content;
				if (is_strlen_equals(var, var_content->var_name))
				{
					if (ft_strncmp(var_content->var_name, var,
					ft_strlen(var)) == 0)
						var_content->is_exported = true;
				}
				tmp_list = tmp_list->next;
			}
			i++;
		}
	}
	return (0);
}

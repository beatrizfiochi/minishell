/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:30:38 by djunho            #+#    #+#             */
/*   Updated: 2025/06/26 15:37:44 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cmd.h"
#include "../execute/exec_utils.h"
#include "../libft/libft.h"
#include "../minishell.h"
#include "builtins.h"

static int	run_builtin(int argc, char *argv[], t_list *var_list, int *ret)
{
	size_t					i;
	const struct s_builtins	cmds[] = {
	{"echo", echo,},
	{"cd", cd,},
	};

	*ret = EXIT_CMD_NOT_FOUND;
	i = 0;
	while ((i < (sizeof(cmds) / sizeof(cmds[0])))
		&& (ft_strncmp(cmds[i].cmd, (const char*)argv[0],
				ft_strlen(cmds[i].cmd) + 1) != 0))
		i++;
	if (i < (sizeof(cmds) / sizeof(cmds[0])))
		*ret = (cmds[i].func(argc, argv, var_list));
	return (*ret);
}

int	execute_builtin(t_cmd *cmd, t_list *var_list)
{
	char	**args;
	int		argc;
	int		ret;

	args = convert_list_to_vector(cmd->tokens, &argc);
	ret = run_builtin(argc, args, var_list, &ret);
	free(args);
	if (ret != EXIT_CMD_NOT_FOUND)
		cmd->is_builtin = true;
	return (ret);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:30:38 by djunho            #+#    #+#             */
/*   Updated: 2025/07/11 22:22:47 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cmd.h"
#include "../execute/exec_utils.h"
#include "../libft/libft.h"
#include "../minishell.h"
#include "builtins.h"

static int	run_builtin(int argc, char *argv[], t_shell *shell, int *ret)
{
	size_t					i;
	const struct s_builtins	cmds[] = {
	{"echo", echo,},
	{"cd", cd,},
	{"env", env,},
	{"export", export},
	{"pwd", pwd},
	};

	*ret = EXIT_CMD_NOT_FOUND;
	i = 0;
	while ((i < (sizeof(cmds) / sizeof(cmds[0])))
		&& (ft_strncmp(cmds[i].cmd, (const char*)argv[0],
				ft_strlen(cmds[i].cmd) + 1) != 0))
		i++;
	if (i < (sizeof(cmds) / sizeof(cmds[0])))
		*ret = (cmds[i].func(argc, argv, shell));
	return (*ret);
}

int	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	char	**args;
	int		argc;
	int		ret;

	args = convert_list_to_vector(cmd->tokens, &argc);
	ret = run_builtin(argc, args, shell, &ret);
	free(args);
	if (ret != EXIT_CMD_NOT_FOUND)
		cmd->is_builtin = true;
	return (ret);
}

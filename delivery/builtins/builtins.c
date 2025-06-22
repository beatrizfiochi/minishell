/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:30:38 by djunho            #+#    #+#             */
/*   Updated: 2025/06/23 14:18:29 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cmd.h"
#include "../execute/execution.h"
#include "../parser/parser.h"
#include "../libft/libft.h"
#include "builtins.h"

static int	run_builtin(int argc, char *argv[], char *envp[], int *ret)
{
	size_t					i;
	const struct s_builtins	cmds[] = {
	{"echo", echo,},
	{"cd", cd,},
	};

	*ret = 127;
	i = 0;
	while ((i < (sizeof(cmds) / sizeof(cmds[0])))
		&& (ft_strncmp(cmds[i].cmd, (const char*)argv[0],
				ft_strlen(cmds[i].cmd) + 1) != 0))
		i++;
	if (i < (sizeof(cmds) / sizeof(cmds[0])))
		*ret = (cmds[i].func(argc, argv, envp));
	return (*ret);
}

int	execute_builtin(t_cmd *cmd, char *envp[])
{
	char	**args;
	int		argc;
	int		ret;

	clean_token_quotes(cmd->tokens);
	args = convert_list_to_vector(cmd->tokens, &argc);
	ret = run_builtin(argc, args, envp, &ret);
	free(args);
	return (ret);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:30:38 by djunho            #+#    #+#             */
/*   Updated: 2025/07/29 21:51:51 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "../cmd.h"
#include "../execute/exec_utils.h"
#include "../execute/execution.h"
#include "../../libft/libft.h"
#include "../minishell.h"
#include "builtins.h"

static t_builtin_func	get_builtin(char *progname)
{
	size_t					i;
	const struct s_builtins	cmds[] = {
	{"echo", echo,},
	{"cd", cd,},
	{"env", env,},
	{"export", export,},
	{"pwd", pwd,},
	{"exit", sh_exit,},
	{"unset", unset,},
	};

	i = 0;
	while ((i < (sizeof(cmds) / sizeof(cmds[0])))
		&& (ft_strncmp(cmds[i].cmd, (const char*)progname,
				ft_strlen(cmds[i].cmd) + 1) != 0))
		i++;
	if (i < (sizeof(cmds) / sizeof(cmds[0])))
		return (cmds[i].func);
	return (NULL);
}

static int	run_builtin(t_list *tokens, t_shell *shell)
{
	t_builtin_func	func;
	char			**argv;
	int				argc;
	int				ret;

	func = get_builtin(tokens->content);
	if (func == NULL)
		return (EXIT_CMD_NOT_FOUND);
	argv = convert_list_to_vector(tokens, &argc);
	ret = func(argc, argv, shell);
	free(argv);
	return (ret);
}

static void	save_fds(int fds[2])
{
	fds[0] = dup(STDIN_FILENO);
	if (fds[0] < 0)
	{
		perror("Error saving stdin");
		fds[0] = -1;
	}
	fds[1] = dup(STDOUT_FILENO);
	if (fds[1] < 0)
	{
		perror("Error saving stdout");
		fds[1] = -1;
	}
}

static void	restore_fds(int fds[2])
{
	if (fds[0] >= 0)
	{
		if (dup2(fds[0], STDIN_FILENO) < 0)
			perror("Error restoring stdin");
		close(fds[0]);
	}
	if (fds[1] >= 0)
	{
		if (dup2(fds[1], STDOUT_FILENO) < 0)
			perror("Error restoring stdout");
		close(fds[1]);
	}
}

int	execute_builtin(t_cmd *cmd, t_shell *shell, bool can_redirect)
{
	int		ret;
	int		fds[2];

	if (get_builtin(cmd->tokens->content) == NULL)
		return (EXIT_CMD_NOT_FOUND);
	if (can_redirect)
	{
		save_fds(fds);
		configure_redir(shell, cmd);
	}
	ret = run_builtin(cmd->tokens, shell);
	if (can_redirect)
		restore_fds(fds);
	if (ret != EXIT_CMD_NOT_FOUND)
		cmd->is_builtin = true;
	return (ret);
}

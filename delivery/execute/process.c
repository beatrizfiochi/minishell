/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:55:45 by djunho            #+#    #+#             */
/*   Updated: 2025/05/21 11:33:23 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>		// pid_t
#include <unistd.h>			// execve, fork, pipe, dup2, close
#include <sys/wait.h>		// wait
#include "../libft/libft.h"
#include "../cmd.h"
#include "execution.h"

static int	run_fork(t_cmd *cmds, char **envp)
{
	char	*path;
	char	**args;

	args = convert_list_to_vector(cmds->tokens);
	if (args == NULL)
		return (0);
	if (args[0] == NULL)
	{
		free_join(args);
		return (0);
	}
	execve(args[0], args, envp);
	if (create_cmd_path(args[0], envp, &path))
		execve(path, args, envp);
	perror(args[0]);
	free_join(args);
	return (127);
}

// static void	run_father(int fd[2], int *fd_tmp, bool is_first, pid_t pid)
// {
// 	(void)pid;
// 	close(fd[1]);
// 	if (!is_first)
// 		close(*fd_tmp);
// 	*fd_tmp = dup(fd[0]);
// 	close(fd[0]);
// }

static int	run_child(int fd[2], int fd_tmp, int i, t_cmd *cmd, t_pipex *pipex)
{
	(void)fd;
	(void)fd_tmp;
	(void)i;
	// close(fd[0]);
	// fd[0] = fd_tmp;
	// if (i == 0)
	// 	fd[0] = pipex->fd1;
	// if (i == pipex->ncmds - 1)
	// {
	// 	close(fd[1]);
	// 	fd[1] = pipex->fd2;
	// }
	// if (fd[0] < 0)
	// 	close(STDIN_FILENO);
	// else
	// {
	// 	dup2(fd[0], STDIN_FILENO);
	// 	close(fd[0]);
	// }
	// if (fd[1] < 0)
	// 	close(STDOUT_FILENO);
	// else
	// {
	// 	dup2(fd[1], STDOUT_FILENO);
	// 	close(fd[1]);
	// }
	//TODO: Fix here!
	return (run_fork(cmd, pipex->envp));
}

static int	btree_operator_callback(t_btnode *node, int ret, void *_pipex)
{
	t_pipex	*pipex;
	int		wstatus;

	pipex = _pipex;
	if (node->content != NULL)
	{
		if (((t_content_node *)node->content)->op == OP_AND)
		{
			if (ret != 0)
			{
				return (ret);
			}
			waitpid(pipex->last_pid, &wstatus, 0);
			if (WIFEXITED(wstatus))
				return (WEXITSTATUS(wstatus));
			else
			{
				printf("Error in child process\n");
			}
		}
		else if (((t_content_node *)node->content)->op == OP_OR)
		{
			if (ret != 0)
			{
				return (0);
			}
			waitpid(pipex->last_pid, &wstatus, 0);
			if (WIFEXITED(wstatus))
				return (WEXITSTATUS(wstatus) == 0);
			else
			{
				printf("Error in child process\n");
			}
		}
	}
	return (-1);
}

__attribute__((weak)) void	debug_btree_print(t_btnode *node)
{
	(void)node;
}

static int	btree_cmd_callback(t_btnode *node, void *_pipex)
{
	t_pipex	*pipex;

	pipex = (t_pipex *)_pipex;
	if (node->content != NULL)
	{
		debug_btree_print(node);
		pipex->last_pid = fork();
		if (pipex->last_pid < 0)
			return (1);
		if (pipex->last_pid == 0)
		{
			exit(run_child(0, 0, 0, &((t_content_node *)node->content)->cmd,
					(t_pipex *)pipex));
		}
	}
	return (0);
}

int	process(t_pipex *pipex)
{
	int		tmp;
	// int		pipe_fd[2];
	int		i;
	// pid_t	pid;

	i = -1;
	btree_foreach_dfs(pipex->cmds, btree_operator_callback, btree_cmd_callback, pipex);
	// while (++i < pipex->ncmds)
	// {
	// 	if (pipe(pipe_fd) < 0)
	// 		return (1);
	// 	pid = fork();
	// 	if (pid < 0)
	// 		return (1);
	// 	if (pid == 0)
	// 		exit(run_child(pipe_fd, tmp, i, pipex));
	// 	run_father(pipe_fd, &tmp, i == 0, pid);
	// }
	tmp = waitpid(pipex->last_pid, &i, 0);
	while ((tmp > 0))
	{
		tmp = wait(NULL);
	}
	if (WIFEXITED(i))
		return (WEXITSTATUS(i));
	return (0);
}

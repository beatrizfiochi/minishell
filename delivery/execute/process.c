/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:55:45 by djunho            #+#    #+#             */
/*   Updated: 2025/05/26 09:32:25 by djunho           ###   ########.fr       */
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
#include "../debug.h"
#include "../minishell.h"
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

static int	run_child(int fd[2], int fd_tmp, int i, t_cmd *cmd, t_shell *shell)
{
	(void)fd;
	(void)fd_tmp;
	(void)i;
	// close(fd[0]);
	// fd[0] = fd_tmp;
	// if (i == 0)
	// 	fd[0] = shell->fd1;
	// if (i == shell->ncmds - 1)
	// {
	// 	close(fd[1]);
	// 	fd[1] = shell->fd2;
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
	return (run_fork(cmd, shell->envp));
}

static int	btree_operator_before_callback(t_btnode *node, int ret, void *_shell)
{
	t_shell			*shell;
	t_content_node	*content;
	t_content_node	*left_content;

	(void)ret;
	(void)node;
	(void)_shell;
	content = (t_content_node *)node->content;
	if (content->op != OP_PIPE)
		return 0;

	// Check the left node if is a CMD
	if (node->left == NULL && node->left->content == NULL)
	{
		printf("Error: Pipe operator without left command\n");
		return (1);
	}
	left_content = (t_content_node *)node->left->content;
	if (left_content->op != OP_CMD)
	{
		content->pipe.pipe[0] = -1;
		content->pipe.pipe[1] = -1;
		return (0);
	}

	shell = _shell;
	(void)shell;
	content->pipe.isLastPipe = false;

	content->pipe.carry_over_fd = -1;
	if (node->parent != NULL)
	{
		t_content_node *parent_content = (t_content_node *)node->parent->content;
		if ((parent_content != NULL) && (parent_content->op == OP_PIPE))
		{
			// content->pipe.carry_over_fd = dup(parent_content->pipe.pipe[0]);
			content->pipe.carry_over_fd = parent_content->pipe.carry_over_fd;
		}
	}

	// The STDIN of the next command is the output pipe

	if (pipe(content->pipe.pipe) < 0)
		return (1);

	// if (content->pipe.carry_over_fd != -1)
	// {
	// 	close(content->pipe.pipe[0]);
	// 	content->pipe.pipe[0] = content->pipe.carry_over_fd;
	// }
	return (0);
}

static int	btree_operator_between_callback(t_btnode *node, int ret, void *_shell)
{
	t_shell	*shell;
	int		wstatus;
	t_content_node	*content;

	shell = _shell;
	content = (t_content_node *)node->content;
	if (ret != 0)
		return (0);
	if (node->content != NULL)
	{
		if (((t_content_node *)node->content)->op == OP_AND)
		{
			waitpid(shell->last_pid, &wstatus, 0);
			if (WIFEXITED(wstatus))
				return (WEXITSTATUS(wstatus));
			else
			{
				printf("Error in child process\n");
			}
		}
		else if (((t_content_node *)node->content)->op == OP_OR)
		{
			waitpid(shell->last_pid, &wstatus, 0);
			if (WIFEXITED(wstatus))
				return (WEXITSTATUS(wstatus) == 0);
			else
			{
				printf("Error in child process\n");
			}
		}
		else if (((t_content_node *)node->content)->op == OP_PIPE)
		{
			t_node_op right_operation = OP_INVALID;
			if (node->right != NULL)
			{
				t_content_node *right_content = (t_content_node *)node->right->content;
				if (right_content != NULL)
					right_operation = right_content->op;
			}

			close(content->pipe.carry_over_fd);
			content->pipe.carry_over_fd = -1;
			// if (node->parent != NULL)
			// {
			// 	t_content_node *parent_content = (t_content_node *)node->parent->content;
			// 	if ((parent_content != NULL) && (parent_content->op == OP_PIPE))
			// 	{
			// 		tmp = dup(parent_content->pipe.pipe[0]);
			// 	}
			// }

			// The STDIN of the next command is the output pipe
			// if (tmp == -1)
			content->pipe.carry_over_fd = dup(content->pipe.pipe[0]);
			close(content->pipe.pipe[0]);
			close(content->pipe.pipe[1]);

			if (pipe(content->pipe.pipe) < 0)
				return (1);
			close(content->pipe.pipe[0]);
			content->pipe.pipe[0] = content->pipe.carry_over_fd;

			if (right_operation != OP_PIPE)
			{
				content->pipe.isLastPipe = true;
			}

			return (0);
		}
	}
	return (-1);
}

__attribute__((weak)) void	debug_btree_print(t_btnode *node)
{
	(void)node;
}

static int	btree_cmd_callback(t_btnode *node, void *_shell)
{
	t_shell	*shell;
	t_content_node	*parent_content;

	shell = (t_shell *)_shell;
	if (node->content != NULL)
	{
		debug_btree_print(node);
		shell->last_pid = fork();
		if (shell->last_pid < 0)
			return (1);
		if (shell->last_pid == 0)
		{
			if ((node->parent != NULL) && ((t_content_node*)node->parent->content)->op == OP_PIPE)
			{
				parent_content = (t_content_node *)node->parent->content;
				if (parent_content->pipe.carry_over_fd != -1)
				{
					dup2(parent_content->pipe.carry_over_fd, STDIN_FILENO);
					close(parent_content->pipe.carry_over_fd);
				}
				else
				{
					close(parent_content->pipe.pipe[0]);
				}

				// dup2(parent_content->pipe.pipe[0], STDIN_FILENO);
				// close(parent_content->pipe.pipe[0]);

				if (!parent_content->pipe.isLastPipe)
				{
					dup2(parent_content->pipe.pipe[1], STDOUT_FILENO);
				} else
					printf("AQUIIIIII\n");
				close(parent_content->pipe.pipe[1]);
			}
			exit(run_child(0, 0, 0, &((t_content_node *)node->content)->cmd, shell));
		}
	}
	return (0);
}

int	process(t_shell *shell)
{
	int		tmp;
	int		i;

	i = -1;
	btree_foreach_before_and_between_dfs(shell->cmds, btree_operator_before_callback, btree_operator_between_callback, btree_cmd_callback, shell);
	tmp = waitpid(shell->last_pid, &i, 0);
	while ((tmp > 0))
	{
		tmp = wait(NULL);
	}
	if (WIFEXITED(i))
		return (WEXITSTATUS(i));
	return (0);
}

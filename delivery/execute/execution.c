/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:53:41 by djunho            #+#    #+#             */
/*   Updated: 2025/07/06 14:58:00 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>			// exit()
#include <stdio.h>			// perror()
#include <stdbool.h>
#include <fcntl.h>			// open
#include <unistd.h>			// close
#include "../libft/libft.h"	// ft_printf
#include "execution.h"

#define HERE_DOC_TMP_FILE		"/tmp/here_doc"

// static	void	check_here_doc(char **file, t_pipex *pipex)
// {
// 	int		fd;
// 	char	*line;
// 	char	*limiter;
//
// 	if (ft_strncmp(*file, "here_doc", 8) != 0)
// 		return ;
// 	*file = HERE_DOC_TMP_FILE;
// 	pipex->append_fd2 = true;
// 	limiter = *pipex->cmds++;
// 	pipex->ncmds--;
// 	fd = open(*file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd < 0)
// 	{
// 		perror("Error to read the here doc");
// 		exit(1);
// 	}
// 	line = get_next_line(STDIN_FILENO);
// 	while (NULL != line && ft_strncmp(line, limiter, ft_strlen(limiter)) != 0)
// 	{
// 		write(fd, line, ft_strlen(line));
// 		free(line);
// 		line = get_next_line(STDIN_FILENO);
// 	}
// 	free(line);
// 	close(fd);
// }

// static	bool	check_files(char *file1, char *file2, t_pipex *pipex)
// {
// 	int	fd2_flag;
//
// 	// check_here_doc(&file1, pipex);
// 	pipex->fd1 = open(file1, O_RDONLY);
// 	if (pipex->fd1 < 0)
// 	{
// 		perror(file1);
// 	}
// 	fd2_flag = O_WRONLY | O_CREAT | O_TRUNC;
// 	if (pipex->append_fd2)
// 		fd2_flag = O_WRONLY | O_CREAT | O_APPEND;
// 	pipex->fd2 = open(file2, fd2_flag, 0644);
// 	if (pipex->fd2 < 0)
// 	{
// 		perror(file2);
// 	}
// 	return (true);
// }

int	execute_execve(t_btnode *node, t_shell *shell)
{
	shell->last_pid = fork();
	if (shell->last_pid < 0)
	{
		perror("Error on fork");
		return (1);
	}
	if (shell->last_pid == 0)
	{
		if (shell->is_running_redirect)
			configure_redirect(shell, &(((t_content_node *)node->content)->cmd));
		exit(run_child(&((t_content_node *)node->content)->cmd, shell));
	}
	if (shell->is_last_redirect)
	{
		shell->is_running_redirect = false;
		shell->is_last_redirect = false;
	}
	return (0);
}

int	get_fork_return(int wstatus)
{
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	else if (WIFSIGNALED(wstatus))
	{
		return (WTERMSIG(wstatus) + 128);
	}
	else if (WIFSTOPPED(wstatus))
	{
		return (WSTOPSIG(wstatus) + 128);
	}
	else
	{
		printf("Error unknown in child process\n");
		return (1);
	}
}

int	execute(t_shell *shell)
{
	shell->ncmds = btree_count_leaves(shell->cmds);
	return (process(shell));
}

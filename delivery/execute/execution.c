/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:53:41 by djunho            #+#    #+#             */
/*   Updated: 2025/05/29 16:40:50 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>			// exit()
#include <stdio.h>			// perror()
#include <stdbool.h>
#include <fcntl.h>			// open
#include <unistd.h>			// close
#include "../libft/libft.h"	// ft_printf
#include "execution.h"
#include "../signals/signals.h"

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

int	execute(t_btnode *tree, t_shell *shell, char *envp[])
{
	shell->ncmds = btree_count_leaves(tree);
	shell->cmds = tree;
	shell->envp = envp;
	shell->last_fd = STDIN_FILENO;
	shell->append_fd2 = false;
	return (process(shell));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   banner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 19:28:29 by djunho            #+#    #+#             */
/*   Updated: 2025/08/18 19:36:00 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "variables/expand_variables.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

static char	*get_header_path(t_shell *shell)
{
	char		*path;
	struct stat	sb;

	path = search_var("HOME", shell->variable_list, 4);
	if (path != NULL)
	{
		path = ft_strjoin(path, "/.config/minishell/header");
		if (path != NULL)
		{
			if (stat(path, &sb) == 0)
				return (path);
			free(path);
		}
	}
	path = ft_strjoin("./", "header");
	return (path);
}

void	print_banner(t_shell *shell)
{
	int		fd;
	char	buf[128];
	int		bytes_read;
	char	*path;

	path = get_header_path(shell);
	if (path == NULL)
		return ;
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		ft_printf("Hello, World of minishell!\n");
		free(path);
		return ;
	}
	bytes_read = read(fd, buf, sizeof(buf));
	while (bytes_read > 0)
	{
		write(STDOUT_FILENO, buf, bytes_read);
		bytes_read = read(fd, buf, sizeof(buf));
	}
	write(STDOUT_FILENO, "\n", 1);
	close(fd);
	free(path);
	return ;
}

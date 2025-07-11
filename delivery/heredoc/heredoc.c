/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:31:10 by djunho            #+#    #+#             */
/*   Updated: 2025/07/11 09:43:50 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>					// open()
#include <readline/readline.h>		// readline()
#include <unistd.h>					// write()
#include "../libft/libft.h"
#include "../btree/btree.h"
#include "../cmd.h"
#include "../color.h"
#include "../minishell.h"
#include "../signals/signals.h"

#define HEREDOC_PROMPT "> "

static char	*get_heredoc_file_name(void)
{
	static int	heredoc_counter = 0;
	char		*file_name;
	char		*id;

	id = ft_itoa(heredoc_counter++);
	if (id == NULL)
		return (NULL);
	file_name = ft_strjoin("/tmp/heredoc_", id);
	free(id);
	return (file_name);
}

static int	process_heredoc(t_shell *shell, t_content_node *content, char **eof)
{
	int			line_start = 0;
	char		*file_name;
	char		*line;
	int			fd;
	int			ret;

	ret = EXIT_SUCCESS;
	line_start = shell->nlines;
	file_name = get_heredoc_file_name();
	fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(file_name);
		perror("Error opening heredoc file");
		return (EXIT_FAILURE);
	}
	heredoc_signals();
	line = sh_read_line(shell, COLOR_MAGENTA HEREDOC_PROMPT COLOR_RESET);
	while ((line != NULL) && (ft_strncmp(line, *eof, ft_strlen(*eof) + 1) != 0))
	{
		if (write(fd, line, ft_strlen(line)) == -1)
		{
			free(file_name);
			free(line);
			return (EXIT_FAILURE);
		}
		write(fd, "\n", 1);
		free(line);
		line = sh_read_line(shell, COLOR_MAGENTA HEREDOC_PROMPT COLOR_RESET);
	}
	if (line == NULL)
	{
		if (g_signal == SIGINT)
			ret = EXIT_SIGNAL_BASE + g_signal;
		else
			ft_fprintf(STDERR_FILENO, "warning: here-document at line %d delimited by end-of-file (wanted `%s')\n",
				line_start, *eof);
	}
	else
		free(line);
	heredoc_ignore_signals();
	close(fd);
	content->op = OP_RD_INPUT;
	free(*eof);
	*eof = file_name;
	return (ret);
}


int	check_tree_for_heredoc(t_shell *shell, t_btnode *node)
{
	int				ret;
	t_content_node *content;
	t_content_node *right_content;

	ret = EXIT_SUCCESS;
	if (btree_is_leaf(node) || (node->right == NULL))
		return (ret);
	content = (t_content_node *)node->content;
	right_content = (t_content_node *)node->right->content;
	if (content->op == OP_HEREDOC)
		ret = process_heredoc(shell, content,
			(char **)(&right_content->cmd.tokens->content)
					);
	else if (ret == EXIT_SUCCESS)
		ret = check_tree_for_heredoc(shell, node->right);
	if ((ret == EXIT_SUCCESS) && (node->left != NULL))
		ret = check_tree_for_heredoc(shell, node->left);
	return (ret);
}


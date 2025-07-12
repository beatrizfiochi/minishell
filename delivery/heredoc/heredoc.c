/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:31:10 by djunho            #+#    #+#             */
/*   Updated: 2025/07/11 21:56:04 by djunho           ###   ########.fr       */
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
#include "heredoc_aux.h"

#define HEREDOC_PROMPT "> "

static int	heredoc_readlines(t_shell *shell, int fd, char *eof,
				bool should_expand)
{
	char	*line;
	int		ret;

	ret = EXIT_SUCCESS;
	line = sh_read_line(shell, COLOR_MAGENTA HEREDOC_PROMPT COLOR_RESET);
	while ((line != NULL) && (ft_strncmp(line, eof, ft_strlen(eof) + 1) != 0))
	{
		ret = EXIT_FAILURE;
		expand_line(shell, should_expand, &line);
		if (write(fd, line, ft_strlen(line)) == -1)
			break ;
		ret = EXIT_SUCCESS;
		write(fd, "\n", 1);
		free(line);
		line = sh_read_line(shell, COLOR_MAGENTA HEREDOC_PROMPT COLOR_RESET);
	}
	if ((line == NULL) && (g_signal == SIGINT))
		ret = EXIT_SIGNAL_BASE + g_signal;
	else if (line == NULL)
		ret = EXIT_SIGNAL_BASE + SIGTERM;
	else
		free(line);
	return (ret);
}

static int	heredoc_doc(t_shell *shell, int fd, char *eof, bool should_expand)
{
	int		line_start;
	int		ret;

	line_start = shell->nlines;
	ret = heredoc_readlines(shell, fd, eof, should_expand);
	if (ret == (EXIT_SIGNAL_BASE + SIGTERM))
	{
		ret = EXIT_SUCCESS;
		ft_fprintf(STDERR_FILENO, "warning: here-document at line %d delimited"
			"by end-of-file (wanted `%s')\n", line_start, eof);
	}
	return (ret);
}

static int	process_heredoc(t_shell *shell, t_content_node *content, char **eof)
{
	char		*file_name;
	int			fd;
	int			ret;

	file_name = get_heredoc_file_name();
	fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(file_name);
		perror("Error opening heredoc file");
		return (EXIT_FAILURE);
	}
	heredoc_signals();
	ret = heredoc_doc(shell, fd, *eof, should_expand(eof));
	heredoc_ignore_signals();
	if (ret != EXIT_SUCCESS)
	{
		free(file_name);
		return (ret);
	}
	close(fd);
	content->op = OP_RD_INPUT;
	free(*eof);
	*eof = file_name;
	return (ret);
}

int	check_tree_for_heredoc(t_shell *shell, t_btnode *node)
{
	int				ret;
	t_content_node	*content;
	t_content_node	*right_content;

	ret = EXIT_SUCCESS;
	if (btree_is_leaf(node) || (node->right == NULL))
		return (ret);
	content = (t_content_node *)node->content;
	right_content = (t_content_node *)node->right->content;
	if (content->op == OP_HEREDOC)
		ret = process_heredoc(shell, content,
				(char **)(&right_content->cmd.tokens->content));
	else if (ret == EXIT_SUCCESS)
		ret = check_tree_for_heredoc(shell, node->right);
	if ((ret == EXIT_SUCCESS) && (node->left != NULL))
		ret = check_tree_for_heredoc(shell, node->left);
	return (ret);
}

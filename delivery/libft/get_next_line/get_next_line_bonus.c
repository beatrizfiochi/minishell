/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 18:47:47 by djunho            #+#    #+#             */
/*   Updated: 2025/01/07 17:31:05 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "get_next_line_bonus.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE	50
#endif // BUFFER_SIZE

#ifndef MAX_FD_ALLOWED
# define MAX_FD_ALLOWED	1024
#endif // MAX_FD_ALLOWED

char	*increase_buffer(char *buffer, int buffer_size, bool should_free)
{
	char	*new_buffer;
	int		i;

	new_buffer = (char *)malloc(buffer_size + BUFFER_SIZE + 1);
	if (new_buffer == NULL)
		return (NULL);
	i = 0;
	while (i < buffer_size)
	{
		new_buffer[i] = buffer[i];
		i++;
	}
	new_buffer[i] = '\0';
	new_buffer[buffer_size + BUFFER_SIZE] = '\0';
	if (should_free)
		free(buffer);
	return (new_buffer);
}

// Return true if found the \n of \0
// Also calculates the new size of str
static bool	find_new_line(char *str, int *str_size, int *n_read)
{
	while ((*n_read > 0) && (str[*str_size] != '\n')
		&& (str[*str_size] != '\0'))
	{
		(*str_size)++;
		(*n_read)--;
	}
	if ((*n_read > 0) && (str[*str_size] == '\n'))
	{
		(*str_size)++;
		(*n_read)--;
	}
	return ((*str_size > 0)
		&& ((str[*str_size - 1] == '\n') || (str[*str_size - 1] == '\0')));
}

// Return a pointer to the string
static char	*cpy_to_internal_buf(char *str, char *buffer,
								int *str_size, int *n_read)
{
	int	i;

	i = 0;
	while (i < *n_read)
	{
		buffer[i] = str[*str_size + i];
		str[*str_size + i] = '\0';
		i++;
	}
	str[*str_size] = '\0';
	buffer[i] = '\0';
	return (str);
}

#define RET_FAIL			-1
#define RET_CONTINUE		0
#define RET_FOUND_LINE		1

static int	gnl_internal(int fd, char **str, int *str_size, int *n_read)
{
	if (find_new_line(*str, str_size, n_read))
	{
		return (RET_FOUND_LINE);
	}
	if ((*n_read == 0) && (*str_size > 0) && ((*str)[*str_size - 1] != '\n'))
	{
		*str = increase_buffer(*str, *str_size, true);
		if (*str == NULL)
			return (RET_FAIL);
	}
	*n_read = read(fd, &(*str)[*str_size], BUFFER_SIZE);
	if ((*n_read < 0) || ((*n_read == 0) && (*str_size == 0)))
	{
		return (RET_FAIL);
	}
	if (*n_read == 0)
	{
		return (RET_FOUND_LINE);
	}
	return (RET_CONTINUE);
}

char	*get_next_line(int fd)
{
	static char	buffer[MAX_FD_ALLOWED][BUFFER_SIZE];
	char		*str;
	int			str_size;
	int			n_read;
	int			ret;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > MAX_FD_ALLOWED)
		return (NULL);
	n_read = gnl_strlen(buffer[fd]);
	str = increase_buffer(buffer[fd], n_read, false);
	if (str == NULL)
		return (NULL);
	str_size = 0;
	while (1)
	{
		ret = gnl_internal(fd, &str, &str_size, &n_read);
		if (ret == RET_FOUND_LINE)
			return (cpy_to_internal_buf(str, buffer[fd], &str_size, &n_read));
		else if (ret == RET_FAIL)
			break ;
	}
	free(str);
	buffer[fd][0] = '\0';
	return (NULL);
}

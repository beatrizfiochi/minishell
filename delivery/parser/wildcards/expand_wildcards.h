/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 21:55:46 by djunho            #+#    #+#             */
/*   Updated: 2025/07/13 21:58:03 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXPAND_WILDCARDS_H
# define EXPAND_WILDCARDS_H

int		ft_strcmp_insensitive(char *s1, char *s2);
int		get_number_files(char *path, char *pattern);
char	**read_dir(char **files, int size, char *path, char *pattern);

#endif // EXPAND_WILDCARDS_H

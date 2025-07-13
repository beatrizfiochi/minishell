/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:56:51 by djunho            #+#    #+#             */
/*   Updated: 2025/07/13 20:15:58 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_UTILS_H
# define ENV_UTILS_H

# include "../libft/libft.h"

const char	*get_env(char *env, t_list *var_list);

char		**convert_list_to_envp(t_list *var_list, t_list *tmp_var_list);

char		**free_envp(char **envp);

void		sort_envp(char **envp, int sort(char *, char *));

int			get_envp_size(char **envp);

char		**envp_cat(char **envp1, char **envp2);

#endif // ENV_UTILS_H

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:56:51 by djunho            #+#    #+#             */
/*   Updated: 2025/06/26 17:11:54 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_UTILS_H
# define ENV_UTILS_H

const char	*get_env(char *env, t_list *var_list);

char		**convert_list_to_envp(t_list *var_list);

void	free_envp(char **envp);

#endif // ENV_UTILS_H

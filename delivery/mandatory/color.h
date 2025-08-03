/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 20:13:28 by djunho            #+#    #+#             */
/*   Updated: 2025/07/11 22:50:59 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef COLOR_H
# define COLOR_H

// The \1 and the \2 are the RL_PROMPT_START_IGNORE and RL_PROMPT_END_IGNORE

# define COLOR_RED     "\1\x1b[31m\2"
# define COLOR_GREEN   "\1\x1b[32m\2"
# define COLOR_YELLOW  "\1\x1b[33m\2"
# define COLOR_BLUE    "\1\x1b[34m\2"
# define COLOR_MAGENTA "\1\x1b[35m\2"
# define COLOR_CYAN    "\1\x1b[36m\2"
# define COLOR_RESET   "\1\x1b[0m\2"

#endif // COLOR_H

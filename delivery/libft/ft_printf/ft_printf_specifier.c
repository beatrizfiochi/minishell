/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_specifier.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 22:23:32 by djunho            #+#    #+#             */
/*   Updated: 2025/05/18 20:54:49 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdarg.h>
#include <stdbool.h>
#include "ft_printf_internals.h"

static bool	get_specifier(const char c, t_flags *flag)
{
	if (!((c == ' ') || (c == '+') || (c == '#') || (c == '.')
			|| (c == '-') || (c == '0') || ((c >= '1') && (c <= '9'))))
		return (false);
	if (!flag->space)
		flag->space = ((c == ' ') || ((c >= '1') && (c <= '9')));
	if (!flag->force_sign)
		flag->force_sign = (c == '+');
	if (!flag->left_aligned)
		flag->left_aligned = (c == '-');
	if (!flag->zero)
		flag->zero = (c == '0');
	if (!flag->precision)
		flag->precision = (c == '.');
	if (!flag->prefix)
		flag->prefix = (c == '#');
	return ((c == ' ') || (c == '+') || (c == '#') || (c == '.')
		|| (c == '-') || (c == '0') || ((c >= '1') && (c <= '9')));
}

static void	get_specifier_width(const char **fmt, t_flags *flag)
{
	unsigned int	mult;
	int				*value;

	value = NULL;
	if ((**fmt == ' ') || ((**fmt >= '0') && (**fmt <= '9'))
		|| (**fmt == '+') || (**fmt == '-'))
		value = &flag->width;
	else if (**fmt == '.')
		value = &flag->precision_width;
	if (value == NULL)
		return ;
	mult = 1;
	if (((**fmt >= '1') && (**fmt <= '9')))
		(*fmt)--;
	*value = 0;
	while ((*(*fmt + 1) >= '0') && (*(*fmt + 1) <= '9'))
	{
		(*fmt)++;
		*value = (*value * mult) + (**fmt - '0');
		mult *= 10;
	}
}

void	check_specifiers(const char **format, t_flags *flag)
{
	bool	get_width;

	get_width = false;
	*flag = (t_flags){.space = false, .force_sign = false, .precision = false,
		.prefix = false, .width = 0, .precision_width = -1,};
	while (get_specifier(**format, flag))
	{
		if ((**format == ' ') || ((**format >= '1') && (**format <= '9')))
			get_width = true;
		else if (**format == '.')
			get_width = true;
		else if (**format == '+')
			get_width = true;
		else if (**format == '-')
			get_width = true;
		else if (**format == '0')
			get_width = true;
		else if (**format == '#')
			flag->prefix = true;
		if (get_width)
			get_specifier_width(format, flag);
		(*format)++;
	}
	if (flag->zero && flag->left_aligned)
		flag->zero = false;
}

void	normalize_flag(t_flags *flag, char conv_spec)
{
	if (flag->precision_width < 0)
	{
		if ((conv_spec == 'd') || (conv_spec == 'i') || (conv_spec == 'u')
			|| (conv_spec == 'x') || (conv_spec == 'X'))
			flag->precision_width = 1;
		else
			flag->precision_width = 0;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_args_numbers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 22:43:39 by djunho            #+#    #+#             */
/*   Updated: 2025/05/18 20:55:24 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "../libft.h"
#include "ft_printf_internals.h"

static int	print_sign(char *s, t_flags const *flag)
{
	int	ret;

	ret = 0;
	if (s[0] == '-')
		write(STDOUT_FILENO, s, sizeof(char));
	else if (flag->force_sign)
	{
		write(STDOUT_FILENO, "+", sizeof(char));
		ret = 1;
	}
	return (ret);
}

int	print_d(int i, t_flags *const flag)
{
	char	*s;
	int		printed;
	int		printed_spec;
	int		printed_prec;

	s = ft_itoa(i);
	printed = ft_strlen(s);
	normalize_number_flag(flag, printed, (i < 0));
	printed_spec = get_width(flag, printed, (i >= 0));
	printed_prec = ((flag->precision) * (flag->precision_width));
	print_pad(((flag->force_sign || flag->space)), ' ', printed_spec);
	printed_spec += print_sign(s, flag);
	print_pad(flag->zero, '0', printed_spec);
	print_pad(flag->precision, '0', printed_prec);
	if (s[0] == '-')
		write(STDOUT_FILENO, &s[1], printed - 1);
	else
		write(STDOUT_FILENO, s, printed);
	print_pad(flag->left_aligned, ' ', printed_spec);
	free(s);
	return (printed + printed_spec + printed_prec);
}

int	print_u(unsigned long int u, t_flags *const flag)
{
	int	printed;
	int	printed_specifier;
	int	printed_prec;

	printed = ft_putunbr_base_fd(u, "0123456789", -1);
	normalize_number_flag(flag, printed, false);
	printed_specifier = get_width(flag, printed, true);
	printed_prec = ((flag->precision) * (flag->precision_width));
	print_pad(flag->space, ' ', printed_specifier);
	print_pad(flag->zero, '0', printed_specifier);
	print_pad(flag->precision, '0', printed_prec);
	ft_putunbr_base_fd(u, "0123456789", STDOUT_FILENO);
	print_pad(flag->left_aligned, ' ', printed_specifier);
	return (printed + printed_specifier + printed_prec);
}

int	print_x(unsigned long int u, bool is_upper, t_flags *const flag)
{
	const char	*base;
	int			printed;
	int			printed_specifier;

	base = "0123456789abcdef";
	if (is_upper)
		base = "0123456789ABCDEF";
	printed = 0;
	printed_specifier = 0;
	printed = ft_putunbr_base_fd(u, base, -1);
	normalize_number_flag(flag, printed, false);
	if (flag->prefix && u != 0)
		printed += 2;
	printed_specifier = get_width(flag, printed, false);
	print_pad(flag->space, ' ', printed_specifier);
	if (flag->prefix && (u != 0) && is_upper)
		write(STDOUT_FILENO, "0X", sizeof(char) * 2);
	if (flag->prefix && (u != 0) && !is_upper)
		write(STDOUT_FILENO, "0x", sizeof(char) * 2);
	print_pad(flag->zero, '0', printed_specifier);
	print_pad(flag->precision, '0', flag->precision_width);
	ft_putunbr_base_fd(u, base, STDOUT_FILENO);
	print_pad(flag->left_aligned, ' ', printed_specifier);
	return (printed + printed_specifier
		+ (flag->precision * flag->precision_width));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_args_numbers_helper.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 22:43:39 by djunho            #+#    #+#             */
/*   Updated: 2025/05/18 20:53:42 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "../libft.h"
#include "ft_printf_internals.h"

int	get_width(const t_flags *const flag, int len, bool is_positive)
{
	int	ret;

	ret = flag->width - len;
	if ((flag->precision) && (ret > flag->precision_width))
		ret -= flag->precision_width;
	if ((flag->force_sign) && (is_positive))
		ret--;
	if ((ret < 0) && (!flag->force_sign) && (flag->space) && (is_positive))
		return (1);
	if (ret < 0)
		return (0);
	return (ret);
}

void	print_pad(bool check, char c, int i)
{
	while ((check) && (i-- > 0))
		write(STDOUT_FILENO, &c, sizeof(char));
}

void	normalize_number_flag(t_flags *flag, int len, bool is_neg)
{
	if ((flag->space) && (flag->precision)
		&& (flag->precision_width > flag->width))
		flag->space = false;
	if (flag->precision && (flag->precision_width > (len - is_neg)))
		flag->precision_width = flag->precision_width + is_neg - len;
	else
		flag->precision_width = 0;
}

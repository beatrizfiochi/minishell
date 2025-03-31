/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 22:43:39 by djunho            #+#    #+#             */
/*   Updated: 2025/01/05 14:03:21 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "../libft.h"
#include "ft_printf_internals.h"

int	print_c(char c, t_flags *const flag)
{
	int	pad;
	int	i;

	pad = 0;
	if ((flag->zero) && (flag->width > 0))
		flag->space = true;
	if ((flag->left_aligned) && (flag->width > 0))
		pad = flag->width - sizeof(char);
	if ((flag->space) && (flag->width > 0))
		pad = flag->width - sizeof(char);
	i = pad;
	while ((flag->space) && (i-- > 0))
		write(STDOUT_FILENO, " ", (sizeof(char) * 1));
	write(STDOUT_FILENO, &c, sizeof(char));
	i = pad;
	while ((flag->left_aligned) && (i-- > 0))
		write(STDOUT_FILENO, " ", sizeof(char));
	return (pad + sizeof(char));
}

int	print_s(char *ori, t_flags *const flag)
{
	int		printed;
	int		pad;
	char	*s;

	s = ori;
	if (s == NULL)
		s = "(null)";
	printed = ft_strlen(s);
	if ((flag != NULL) && (flag->precision && flag->precision_width < printed))
		printed = flag->precision_width;
	if ((ori == NULL) && (printed < 6))
		printed = 0;
	pad = 0;
	if ((flag != NULL) && (flag->width >= printed))
		pad = flag->width - printed;
	if ((flag != NULL) && (flag->zero) && (flag->width > 0))
		flag->space = true;
	print_pad((flag != NULL) && (flag->space), ' ', pad);
	write(STDOUT_FILENO, s, (sizeof(char) * printed));
	print_pad((flag != NULL) && (flag->left_aligned), ' ', pad);
	return (printed + pad);
}

int	print_p(unsigned long int u, const t_flags *const flag)
{
	t_flags	new_flag;

	new_flag = *flag;
	new_flag.prefix = true;
	if (u == 0)
		return (print_s("(nil)", NULL));
	else
		return (print_x(u, false, &new_flag));
}

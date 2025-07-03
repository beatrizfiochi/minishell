/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 22:23:32 by djunho            #+#    #+#             */
/*   Updated: 2024/12/01 16:50:49 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdarg.h>
#include "ft_printf_internals.h"

static int	handle_arg(int fd, va_list *arg, const char **format)
{
	int		printed;
	t_flags	flag;

	check_specifiers(format, &flag);
	normalize_flag(&flag, **format);
	printed = 1;
	if (**format == 'c')
		printed = print_c(fd, (char)va_arg(*arg, int), &flag);
	else if (**format == 's')
		printed = print_s(fd, va_arg(*arg, char *), &flag);
	else if (**format == 'd' || **format == 'i')
		printed = print_d(fd, va_arg(*arg, int), &flag);
	else if (**format == 'u')
		printed = print_u(fd, va_arg(*arg, unsigned int), &flag);
	else if (**format == 'x')
		printed = print_x(fd, va_arg(*arg, unsigned int), false, &flag);
	else if (**format == 'X')
		printed = print_x(fd, va_arg(*arg, unsigned int), true, &flag);
	else if (**format == 'p')
		printed = print_p(fd, va_arg(*arg, unsigned long int), &flag);
	else
		write(fd, *format, sizeof(char));
	(*format)++;
	return (printed);
}

int	ft_vdprintf(int fd, const char *format, va_list *arg)
{
	int		ret;
	int		printed;

	printed = 0;
	while (*format != '\0')
	{
		if (*format != '%')
		{
			write(fd, format++, sizeof(char));
			printed++;
			continue ;
		}
		format++;
		ret = handle_arg(fd, arg, &format);
		if (ret < 0)
			break ;
		printed += ret;
	}
	return (printed);
}

int	ft_fprintf(int fd, const char *format, ...)
{
	va_list	arg;
	int		printed;

	printed = 0;
	va_start(arg, format);
	printed = ft_vdprintf(fd, format, &arg);
	va_end(arg);
	return (printed);
}

int	ft_printf(const char *format, ...)
{
	va_list	arg;
	int		printed;

	printed = 0;
	va_start(arg, format);
	printed = ft_vdprintf(STDOUT_FILENO, format, &arg);
	va_end(arg);
	return (printed);
}

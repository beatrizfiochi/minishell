/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_internals.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 22:45:43 by djunho            #+#    #+#             */
/*   Updated: 2024/12/06 12:24:20 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_INTERNALS_H
# define FT_PRINTF_INTERNALS_H

# include <stdbool.h>
# include <stdarg.h>

typedef struct s_flags
{
	bool	space;
	bool	left_aligned;
	bool	force_sign;
	bool	prefix;
	bool	precision;
	bool	zero;
	int		width;
	int		precision_width;
}	t_flags;

void	check_specifiers(const char **format, t_flags *flag);
void	normalize_flag(t_flags *flag, char conv_spec);
int		ft_putnbr_base_fd(long long int nbr, char *base, int fd);
int		ft_putunbr_base_fd(unsigned long long int nbr,
			const char *base, int fd);
int		print_c(int fd, char c, t_flags *const flag);
int		print_s(int fd, char *s, t_flags *const flag);
int		print_d(int fd, int i, t_flags *const flag);
int		print_u(int fd, unsigned long int u, t_flags *const flag);
int		print_x(int fd, unsigned long int u, bool is_upper,
			t_flags *const flag);
int		print_p(int fd, unsigned long int u, const t_flags *const flag);
int		get_width(const t_flags *const flag, int len, bool is_positive);
void	print_pad(int fd, bool check, char c, int i);
void	normalize_number_flag(t_flags *flag, int len, bool is_neg);

#endif // FT_PRINTF_INTERNALS_H

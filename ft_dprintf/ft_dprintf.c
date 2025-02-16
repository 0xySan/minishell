/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 18:08:45 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/16 02:53:48 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_handle_format(va_list args, const char *str, t_val *val)
{
	char	format;

	format = str[val->start + 1];
	if (format == '%')
		val->len += write(val->fd, "%%", 1);
	else if (format == 'c')
		val->len += ft_putchar_fd(va_arg(args, int), val->fd);
	else if (format == 's')
		val->len += ft_putstr_fd(va_arg(args, char *), val->fd);
	else if (format == 'd' || format == 'i')
		val->len += ft_dtoa(va_arg(args, int), val->fd);
	else if (format == 'x')
		ft_ltoa(va_arg(args, unsigned int), "0123456789abcdef", &val->len,
			val->fd);
	else if (format == 'X')
		ft_ltoa(va_arg(args, unsigned int), "0123456789ABCDEF", &val->len,
			val->fd);
	else if (format == 'u')
		val->len += ft_utoa(va_arg(args, unsigned int), val->fd);
	else if (format == 'p')
		val->len += ft_ptoa(va_arg(args, void *), val->fd);
	val->start += 2;
}

int	ft_dprintf(int fd, const char *str, ...)
{
	va_list	args;
	t_val	val;

	val.start = 0;
	val.len = 0;
	val.fd = fd;
	va_start(args, str);
	while (str[val.start])
	{
		if (str[val.start] == '%')
			ft_handle_format(args, str, &val);
		else
			val.len += write(fd, &str[val.start++], 1);
	}
	va_end(args);
	return (val.len);
}

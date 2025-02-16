/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ltoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:25:36 by etaquet           #+#    #+#             */
/*   Updated: 2024/11/20 13:19:03 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_ltoa(long nbr, const char *base, int *len, int fd)
{
	long	base_len;

	if (nbr < 0)
	{
		write(fd, "-", 1);
		nbr = -nbr;
	}
	base_len = 0;
	while (base[base_len])
		base_len++;
	if (nbr >= base_len)
		ft_ltoa(nbr / base_len, base, len, fd);
	*len += ft_putchar_fd(base[nbr % base_len], fd);
}

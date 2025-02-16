/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ultoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:05:45 by etaquet           #+#    #+#             */
/*   Updated: 2024/11/20 13:19:43 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_ultoa(unsigned long nbr, const char *base, int *len, int fd)
{
	unsigned long	base_len;

	base_len = -1;
	while (base[++base_len])
		;
	if (nbr >= base_len)
		ft_ultoa(nbr / base_len, base, len, fd);
	*len += ft_putchar_fd(base[nbr % base_len], fd);
}

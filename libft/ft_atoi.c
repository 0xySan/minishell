/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 22:49:20 by oxy               #+#    #+#             */
/*   Updated: 2024/10/15 16:45:46 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	start;
	int	signe;
	int	value;

	signe = 1;
	start = 0;
	value = 0;
	while (nptr[start] == ' ' || (nptr[start] >= '\t' && nptr[start] <= '\r'))
		start++;
	if (nptr[start] == '-' || nptr[start] == '+')
	{
		if (nptr[start] == '-')
			signe *= -1;
		start++;
	}
	while (nptr[start] >= '0' && nptr[start] <= '9')
		value = value * 10 + nptr[start++] - '0';
	return (value * signe);
}

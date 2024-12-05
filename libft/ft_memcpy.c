/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 12:55:58 by etaquet           #+#    #+#             */
/*   Updated: 2024/10/14 11:53:30 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	v;

	if (!dest || !src)
		return (dest);
	v = -1;
	while (++v < n)
		((char *)dest)[v] = ((char *)src)[v];
	return (dest);
}

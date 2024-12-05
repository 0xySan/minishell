/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 16:43:55 by etaquet           #+#    #+#             */
/*   Updated: 2024/10/15 14:50:15 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_strncpy(char *dest, const char *src, unsigned int n)
{
	unsigned int	v;

	if (!src)
		return ;
	v = -1;
	while (++v < n && src[v])
		dest[v] = src[v];
	dest[v] = '\0';
}

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	start;

	start = -1;
	while (dest && dest[++start])
		;
	if (start >= size)
		return (size + ft_strlen(src));
	ft_strncpy(&dest[start], src, size - start - 1);
	return (start + ft_strlen(src));
}

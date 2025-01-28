/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 16:43:55 by etaquet           #+#    #+#             */
/*   Updated: 2025/01/28 16:17:08 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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

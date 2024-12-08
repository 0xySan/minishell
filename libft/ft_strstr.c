/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 13:03:59 by etaquet           #+#    #+#             */
/*   Updated: 2024/12/08 16:12:39 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *big, const char *little)
{
	size_t	llen;
	int		i;

	llen = ft_strlen(little);
	if (llen == 0)
		return ((char *)big);
	i = 0;
	while (big[i] && llen)
	{
		if (ft_strncmp(&big[i], little, llen) == 0)
			return ((char *)(&big[i]));
		i++;
	}
	return (0);
}

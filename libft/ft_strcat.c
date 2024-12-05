/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 10:30:55 by etaquet           #+#    #+#             */
/*   Updated: 2024/11/04 10:31:09 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcat(char *dest, char *src)
{
	int	start;

	if (!src)
		return (dest);
	start = 0;
	while (dest && dest[start])
		start++;
	ft_strcpy(&dest[start], src);
	return (dest);
}

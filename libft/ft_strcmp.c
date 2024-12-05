/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 10:23:38 by etaquet           #+#    #+#             */
/*   Updated: 2024/11/04 10:23:47 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	start;

	if (!s1 || !s2)
		return (0);
	start = -1;
	while (s1[++start])
	{
		if (s1[start] != s2[start])
			return (s1[start] - s2[start]);
	}
	return (s1[start] - s2[start]);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 00:23:55 by etaquet           #+#    #+#             */
/*   Updated: 2024/10/15 15:43:37 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	start;
	char	*r_value;

	start = 0;
	r_value = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (r_value == NULL)
		return (NULL);
	while (s[start])
	{
		r_value[start] = f(start, s[start]);
		start++;
	}
	r_value[start] = '\0';
	return (r_value);
}

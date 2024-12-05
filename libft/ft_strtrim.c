/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 18:44:30 by etaquet           #+#    #+#             */
/*   Updated: 2024/10/14 11:20:50 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	len;
	char	*r_value;

	if (!s1 || !set)
		return (NULL);
	i = -1;
	while (s1[++i] && ft_strchr(set, s1[i]))
		;
	len = ft_strlen(&s1[i]);
	if (len != 0)
		while (s1[i + len - 1] && ft_strchr(set, s1[i + len - 1]))
			len--;
	r_value = ft_substr(s1, i, len);
	return (r_value);
}

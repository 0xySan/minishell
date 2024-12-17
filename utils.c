/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 15:51:08 by etaquet           #+#    #+#             */
/*   Updated: 2024/12/16 20:40:54 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

void	copy_then_cat(char *dest, char *fstr, char *sstr)
{
	ft_strcpy(dest, fstr);
	ft_strcat(dest, sstr);
}

char	*dup_then_cat(char *src, char *sec_src)
{
	char	*dest;
	int		len;
	int		start;

	if (!src)
		return (0);
	dest = malloc(sizeof(char) * (ft_strlen(src) + ft_strlen(sec_src) + 1));
	if (!dest)
		return (0);
	start = -1;
	while (src[++start])
		dest[start] = src[start];
	dest[start] = '\0';
	ft_strcat(dest, sec_src);
	return (dest);
}

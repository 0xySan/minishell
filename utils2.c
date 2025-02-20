/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:46:49 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/20 16:48:00 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_join(const char *s1, const char *s2, const char *s3)
{
	char	*result;
	int		len;

	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	result = malloc(sizeof(char) * (len + 1));
	ft_strcpy((char *)result, (char *)s1);
	ft_strcat((char *)result, (char *)s2);
    ft_strcat((char *)result, (char *)s3);
	return (result);
}

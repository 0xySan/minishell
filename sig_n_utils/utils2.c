/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:46:49 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/23 04:35:30 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Concatenates three strings.
 * @param s1 First string.
 * @param s2 Second string.
 * @param s3 Third string.
 * @return The concatenated string or NULL on failure.
 */
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

void	edit_lvl(char **env)
{
	char	*lvl;
	int		i;
	int		lvl_int;
	char	*itoa_value;

	lvl = ft_getenv(env, "SHLVL");
	if (!lvl)
		return ;
	lvl_int = ft_atoi(lvl);
	lvl_int++;
	i = -1;
	itoa_value = ft_itoa(lvl_int);
	ft_change_env(env, "SHLVL", itoa_value);
	free(itoa_value);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 20:06:53 by etaquet           #+#    #+#             */
/*   Updated: 2025/01/02 17:36:13 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_getenv(char **env, char *search_env)
{
	int		i;
	char	*test;

	i = 0;
	test = malloc((ft_strlen(search_env) + 2) * sizeof(char));
	copy_then_cat(test, search_env, "=");
	while (env[i])
	{
		if (!ft_strncmp(env[i], test, ft_strlen(test)))
			return (free(test), ft_strchr(env[i], '=') + 1);
		i++;
	}
	return (free(test), NULL);
}

void	ft_show_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

void	ft_free_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

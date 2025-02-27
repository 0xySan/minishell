/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 20:06:53 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/27 15:15:59 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_getenv(char **env, char *search_env)
{
	int		i;
	char	*test;
	char	*value;

	i = 0;
	test = malloc((ft_strlen(search_env) + 2) * sizeof(char));
	if (!test)
		return (NULL);
	copy_then_cat(test, search_env, "=");
	while (env[i])
	{
		if (!ft_strncmp(env[i], test, ft_strlen(test)))
		{
			value = ft_strchr(env[i], '=') + 1;
			free(test);
			return (value);
		}
		i++;
	}
	return (free(test), NULL);
}

char	*ft_spe_getenv(char **env, char *search_env)
{
	int		i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], search_env, ft_strlen(search_env)))
		{
			if (env[i][ft_strlen(search_env)] == '\0')
				return (search_env);
		}
		i++;
	}
	return (NULL);
}

void	ft_show_env(char **env, int *exit_code)
{
	int	i;

	i = 0;
	*exit_code = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
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

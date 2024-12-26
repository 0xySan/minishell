/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 20:06:53 by etaquet           #+#    #+#             */
/*   Updated: 2024/12/26 16:55:37 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <string.h>

char	**dup_all_env(char **env, int size)
{
	char	**new_env;
	int		i;

	i = 0;
	new_env = malloc((count_args(env) + 1 + size) * sizeof(char *));
	if (!new_env)
		return (NULL);
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

char	**dup_all_env_and_free(char **env, int size)
{
	char	**new_env;
	int		i;

	i = 0;
	new_env = malloc((count_args(env) + 1 + size) * sizeof(char *));
	if (!new_env)
		return (NULL);
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		free(env[i]);
		i++;
	}
	free(env);
	new_env[i] = NULL;
	return (new_env);
}

void	ft_change_env(char **env, char *old_env, char *new_env)
{
	int		i;
	char	*start;
	char	*test;

	i = 0;
	test = malloc((ft_strlen(old_env) + 2) * sizeof(char));
	copy_then_cat(test, old_env, "=");
	while (env[i])
	{
		if (!ft_strncmp(env[i], test, ft_strlen(test)))
			break ;
		i++;
	}
	if (!env[i])
		return (free(test));
	start = ft_substr(env[i], 0, ft_strlen(old_env) + 1);
	free(env[i]);
	env[i] = NULL;
	env[i] = dup_then_cat(start, new_env);
	free(start);
	free(test);
}

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

char	**realloc_tab(char ***env)
{
	char	**n_tab;

	n_tab = malloc((count_args(*env) + 2) * sizeof(char *));
	memcpy(n_tab, *env, (count_args(*env) + 1) * sizeof(char *));
	free(*env);
	return (n_tab);
}

void	ft_show_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		dprintf(1, "%s\n", env[i]);
		i++;
	}
}

void	ft_export(char ***env, char *old_env, char *new_env)
{
	int	i;

	if (ft_getenv(*env, old_env))
		return (ft_change_env(*env, old_env, new_env));
	i = count_args(*env);
	*env = realloc_tab(env);
	(*env)[i] = malloc(1 * (ft_strlen(old_env) + ft_strlen(new_env) + 2));
	copy_then_cat((*env)[i], old_env, "=");
	ft_strcat((*env)[i], new_env);
	(*env)[i + 1] = NULL;
}

void	ft_unset(char **env, char *rev_env)
{
	int		i;
	char	*test;

	if (!ft_getenv(env, rev_env))
		return ;
	i = 0;
	test = malloc((ft_strlen(rev_env) + 2) * sizeof(char));
	copy_then_cat(test, rev_env, "=");
	while (env[i])
	{
		if (!ft_strncmp(env[i], test, ft_strlen(test)))
			break ;
		i++;
	}
	if (!env[i])
		return (free(test));
	while (env[i + 1])
	{
		free(env[i]);
		env[i] = ft_strdup(env[i + 1]);
		i++;
	}
	env[i] = NULL;
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

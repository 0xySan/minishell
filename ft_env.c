/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 20:06:53 by etaquet           #+#    #+#             */
/*   Updated: 2024/12/17 16:08:22 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

char	**dup_all_env(char **env)
{
	char	**new_env;
	int		i;

	i = 0;
	new_env = malloc((count_args(env) + 1) * sizeof(char *));
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

void	ft_change_env(char **env, char *old_env, char *new_env)
{
	int		i;
	char	*start;
	char	*test;

	i = 0;
	test = malloc(ft_strlen(old_env) + 2);
	copy_then_cat(test, old_env, "=");
	while (env[i])
	{
		if (ft_strstr(env[i], test))
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
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (env[i])
	{
		if (ft_strstr(env[i], search_env))
		{
			return (ft_strchr(env[i], '=') + 1);
		}
		i++;
	}
	return (NULL);
}

void	ft_export(char **env, char *old_env, char *new_env)
{
	// search if old_env exists, if it does, re-alloc *env and dup the new_env
	// if it doesn't exist re-alloc **env and add 1 to add new_env
}

void	ft_unset(char **env, char *rev_env)
{
	// find if rev_env is in env, if it is, remove that line, free it, and move up all the others.
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

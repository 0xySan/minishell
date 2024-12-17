/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 20:06:53 by etaquet           #+#    #+#             */
/*   Updated: 2024/12/17 01:13:57 by etaquet          ###   ########.fr       */
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
	while(env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	return (new_env);
}

void	ft_change_env(char **env, char *old_env, char *new_env)
{
	int	i;
	char	*start;
	char	*test;

	i = 0;
	while(env[i])
	{
		if (ft_strstr(env[i], old_env))
			break;
		i++;
	}
	if (!env[i])
		return ;
	start = ft_substr(env[i], 0, ft_strlen(old_env) + 1);
	free(env[i]);
	env[i] = dup_then_cat(start, new_env);
}

void	ft_export(char **env, char *old_env, char *new_env)
{
	// search if old_env exists, if it does, re-alloc *env and dup the new_env
	// if it doesn't exist re-alloc **env and add 1 to add new_env
}

char	*ft_getenv(char **env, char *search_env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strstr(env[i], search_env))
			return (ft_strchr(env[i], '='));
		i++;
	}
	return (NULL);
}
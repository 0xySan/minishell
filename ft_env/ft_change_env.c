/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_change_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 17:33:54 by etaquet           #+#    #+#             */
/*   Updated: 2025/01/02 17:36:26 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 17:32:45 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/10 23:39:01 by hdelacou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Duplicate all the environment variables.
 * @param env The environment variables.
 * @return Duplicate of env.
 */
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

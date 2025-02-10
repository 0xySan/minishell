/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_change_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 17:33:54 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/10 23:40:07 by hdelacou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Change an environment variable's value.
 * @param env Array of environment variables.
 * @param old_env Name of the variable to change.
 * @param new_env New value for the variable.
 * If variable not found, does nothing.
 */
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

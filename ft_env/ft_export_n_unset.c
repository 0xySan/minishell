/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_n_unset.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 17:35:19 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/18 09:41:49 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Reallocates memory for a new environment variable.
 * @param env The pointer to the environment variable array.
 * @return The reallocated environment variable array.
 */
char	**realloc_tab(char ***env)
{
	char	**n_tab;

	n_tab = malloc((count_args(*env) + 2) * sizeof(char *));
	memcpy(n_tab, *env, (count_args(*env) + 1) * sizeof(char *));
	free(*env);
	return (n_tab);
}

void	ft_show_export(char **env)
{
	int		n;
	char	**sorted;
	int		i;

	n = count_args(env);
	sorted = malloc(sizeof(char *) * (n + 1));
	copy_array(sorted, env);
	sort_strings(sorted);
	i = 0;
	while (sorted[i])
		printf("export %s\n", sorted[i++]);
	free(sorted);
}

/**
 * @brief Adds/modifies an environment variable.
 * @param env Pointer to the environment array.
 * @param old_env Variable name to update/add.
 * @param new_env New value for the variable.
 */
void	ft_export(char ***env, char *old_env, char *new_env)
{
	int	i;

	if (old_env && !ft_stralnum(old_env))
		return ;
	if (!old_env && !new_env)
		return (ft_show_export(*env));
	if (ft_getenv(*env, old_env))
		return (ft_change_env(*env, old_env, new_env));
	i = count_args(*env);
	*env = realloc_tab(env);
	(*env)[i] = malloc(1 * (ft_strlen(old_env) + ft_strlen(new_env) + 2));
	copy_then_cat((*env)[i], old_env, "=");
	ft_strcat((*env)[i], new_env);
	(*env)[i + 1] = NULL;
}

void	ft_unset_helper(char **env, char *rev_env)
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
	free(env[i]);
	free(test);
	env[i] = NULL;
}

/**
 * @brief Remove an environment variable from the environment array.
 * @param env The environment variables.
 * @param rev_env The name of the variable to remove.
 */
void	ft_unset(char **env, char **rev_env)
{
	int		i;
	int		args;

	i = 0;
	while (rev_env[i])
	{
		ft_unset_helper(env, rev_env[i]);
		i++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 06:40:25 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/18 06:44:20 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*export_util_func(char *word)
{
	int		i;
	int		j;
	char	*r_value;

	i = 0;
	j = 0;
	while (word[i] && word[i] != '=')
		i++;
	r_value = malloc((i + 1) * sizeof(char));
	if (!r_value)
		return (NULL);
	while (j < i)
	{
		r_value[j] = word[j];
		j++;
	}
	r_value[j] = '\0';
	return (r_value);
}

void	check_exportalnum(char **cmd, char ***env)
{
	char	*export_util;

	if (ft_strchr(cmd[1], '='))
		export_util = export_util_func(cmd[1]);
	else
		export_util = ft_strdup(cmd[1]);
	if (!ft_stralnum(export_util))
	{
		g_exit_status = 1 << 8;
		dprintf(2, "export: not valid in this context: %s\n", export_util);
	}
	else
		g_exit_status = 0;
	free(export_util);
}

/**
 * @brief Execute the export command.
 * @param cmd The array of command arguments.
 * @param env The environment variables.
 * @return 1 if the command is recognized and executed, 0 otherwise.
 * Handles the export command. Prints current environment if no args provided;
 * adds variables to environment if args are given.
 */
int	execute_ft_cmds_export(char **cmd, char ***env)
{
	char	*export_util;
	int		i;
	char	*test;

	i = 1;
	if (!cmd[i])
		return (ft_export(env, NULL, NULL), 1);
	check_exportalnum(cmd, env);
	while (cmd[i])
	{
		if (!ft_strchr(cmd[i++], '='))
			continue ;
		i--;
		export_util = export_util_func(cmd[i]);
		if (ft_strlen(export_util) == 0)
		{
			free(export_util);
			i++;
			continue ;
		}
		ft_export(env, export_util, ft_strchr(cmd[i], '=') + 1);
		free(export_util);
		i++;
	}
	return (1);
}

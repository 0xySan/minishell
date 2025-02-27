/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 06:40:25 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/27 15:23:02 by etaquet          ###   ########.fr       */
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

void	check_exportalnum(char **cmd, int *exit_code)
{
	char	*export_util;

	if (ft_strchr(cmd[1], '='))
		export_util = export_util_func(cmd[1]);
	else
		export_util = ft_strdup(cmd[1]);
	if (!ft_stralnum(export_util) || !ft_isalpha(cmd[1][0]))
	{
		*exit_code = 1 << 8;
		dprintf(2, "21sh: export: `%s\': not a valid identifier\n", cmd[1]);
	}
	else
		*exit_code = 0;
	free(export_util);
}

int	execute_ft_cmds_export(char **cmd, char ***env, int *exit_code)
{
	char	*export_util;
	int		i;

	i = 1;
	if (!cmd[i])
		return (ft_export(env, NULL, NULL), 1);
	check_exportalnum(cmd, exit_code);
	while (cmd[i])
	{
		if (!ft_strchr(cmd[i], '='))
		{
			ft_export(env, cmd[i++], NULL);
			continue ;
		}
		export_util = export_util_func(cmd[i]);
		if (ft_strlen(export_util) == 0)
		{
			free(export_util);
			i++;
			continue ;
		}
		ft_export(env, export_util, ft_strchr(cmd[i++], '=') + 1);
		free(export_util);
	}
	return (1);
}

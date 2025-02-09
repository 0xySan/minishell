/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oxy <oxy@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:50:52 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/09 22:00:58 by oxy              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cpath(char *args, char *envpath)
{
	char	*cpath;

	cpath = malloc(sizeof(char) * (ft_strlen(envpath) + ft_strlen(args) + 2));
	if (!cpath)
		return (NULL);
	ft_strcpy(cpath, envpath);
	ft_strcat(cpath, "/");
	ft_strcat(cpath, args);
	return (cpath);
}

char	*get_cmd_path(char *arg, char *path)
{
	char	*cpath;
	char	**mp;
	int		i;

	i = 0;
	if (access(arg, X_OK) != -1 && count_chars(arg, '/') >= 1)
		return (ft_strdup(arg));
	mp = ft_split(path, ':');
	if (!mp)
		return (NULL);
	while (mp[i])
	{
		cpath = get_cpath(arg, mp[i]);
		if (access(cpath, X_OK) != -1 && !ft_strstr(cpath, "//"))
			return (free_args(mp), cpath);
		free(cpath);
		i++;
	}
	free_args(mp);
	return (NULL);
}

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

int	execute_ft_cmds_export(char **cmd, char ***env)
{
	char	*export_util;

	if (!ft_strncmp(cmd[0], "export", 7))
	{
		if (!cmd[1])
		{
			ft_export(env, NULL, NULL);
		}
		else
		{
			if (!ft_strchr(cmd[1], '='))
				return (ft_change_env(*env, "_", cmd[count_args(cmd) - 1]), 1);
			export_util = export_util_func(cmd[1]);
			ft_export(env, export_util, ft_strchr(cmd[1], '=') + 1);
			free(export_util);
		}
		return (1);
	}
	return (0);
}

int	execute_ft_cmds(char **cmd, char ***env)
{
	if (ft_cd(cmd, *env))
		return (1);
	if (!ft_strncmp(cmd[0], "unset", 6))
		return (ft_unset(*env, cmd[1]), 1);
	if (!ft_strncmp(cmd[0], "pwd", 4))
		return (printf("%s\n", ft_getenv(*env, "PWD")), 1);
	if (!ft_strncmp(cmd[0], "echo", 5))
		return (ft_echo(cmd), 1);
	if (!ft_strncmp(cmd[0], "env", 4))
		return (ft_show_env(*env), 1);
	if (execute_ft_cmds_export(cmd, env))
		return (1);
	return (0);
}

void	execute_input(char ***env, char *input, int *exit_status)
{
	char	**cmds;
	char	**cmd;
	char	*parsed_input;
	int		cmd_count;

	parsed_input = preprocess_input(input, *env, exit_status);
	if (!parsed_input)
		return (free_args(cmd));
	cmds = ft_split(parsed_input, ' ');
	cmd = split_array(cmds, "<|>");
	cmd_count = count_args(cmd);
	free_args(cmds);
	free(parsed_input);
	if (ft_count_commands(cmd, cmd_count) == 1)
	{
		if (execute_ft_cmds(cmd, env))
		{
			ft_change_env(*env, "_", cmd[count_args(cmd) - 1]);
			free_args(cmd);
			return ;
		}
	}
	ft_change_env(*env, "_", cmd[count_args(cmd) - 1]);
	ft_parse_pipeline(cmd, cmd_count, *env, exit_status);
	free_args(cmd);
}

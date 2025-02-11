/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:50:52 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/10 23:57:14 by hdelacou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Splits env var into key and value and returns the value.
 * Returns NULL on allocation failure.
 */
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

/**
 * @brief Execute a built-in command.
 * @param cmd The array of command arguments.
 * @param env The environment variables.
 * @return 1 if the command is recognized and executed, 0 otherwise.
 */
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

/**
 * Execute a single command line.
 * @param env The environment variables.
 * @param input The command line to execute.
 * @param exit_status The exit status of the last command.
 * Processes input, splits into commands, executes.
 */
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
			*exit_status = 0;
			return (free_args(cmd));
		}
	}
	ft_change_env(*env, "_", cmd[count_args(cmd) - 1]);
	ft_parse_pipeline(cmd, cmd_count, *env, exit_status);
	free_args(cmd);
}

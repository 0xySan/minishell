/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:50:52 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/16 08:18:13 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	{
		printf("%s\n", ft_getenv(*env, "PWD"));
		g_exit_status = 0;
		return (1);
	}
	if (!ft_strncmp(cmd[0], "echo", 5))
		return (ft_echo(cmd), 1);
	if (!ft_strncmp(cmd[0], "env", 4))
		return (ft_show_env(*env), 1);
	if (!ft_strncmp(cmd[0], "export", 7))
		return (execute_ft_cmds_export(cmd, env), 1);
	return (0);
}

/**
 * Execute a single command line.
 * @param env The environment variables.
 * @param input The command line to execute.
 * Processes input, splits into commands, executes.
 */
void	execute_input(char ***env, char *input)
{
	char	**cmd;
	int		cmd_count;

	cmd = parse_input(input, *env);
	if (cmd == NULL || cmd[0] == NULL)
		return ;
	cmd_count = count_args(cmd);
	if (ft_count_commands(cmd, cmd_count) == 1)
	{
		if (execute_ft_cmds(cmd, env))
		{
			ft_change_env(*env, "_", cmd[cmd_count - 1]);
			return (free_args(cmd));
		}
	}
	ft_change_env(*env, "_", cmd[cmd_count - 1]);
	ft_parse_pipeline(cmd, cmd_count, *env);
	free_args(cmd);
}

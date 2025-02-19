/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:50:52 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/19 17:19:40 by etaquet          ###   ########.fr       */
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
	char	*current_pwd;

	if (ft_cd(cmd, env))
		return (1);
	if (!ft_strncmp(cmd[0], "unset", 6))
		return (ft_unset(*env, cmd), 1);
	if (!ft_strncmp(cmd[0], "pwd", 4))
	{
		current_pwd = ft_get_current_dir();
		ft_dprintf(1, "%s\n", current_pwd);
		if (current_pwd)
			free(current_pwd);
		g_exit_status = 0;
		return (1);
	}
	if (!ft_strncmp(cmd[0], "echo", 5))
		return (ft_echo(cmd), 1);
	if (!ft_strncmp(cmd[0], "env", 4))
		return (ft_show_env(*env), 1);
	if (!ft_strncmp(cmd[0], "export", 7))
		return (execute_ft_cmds_export(cmd, env), 1);
	if (!ft_strncmp(cmd[0], "exit", 5))
		return (2);
	return (0);
}

/**
 * Execute a single command line.
 * @param env The environment variables.
 * @param input The command line to execute.
 * Processes input, splits into commands, executes.
 */
int	execute_input(char ***env, char *input, char *cwd, t_free *free_value)
{
	char	**cmd;
	int		cmd_count;
	int		exit_c;

	cmd = parse_input(input, *env);
	free_value->env = env;
	free_value->parsed_input = cmd;
	if (cmd == NULL || cmd[0] == NULL)
		return (0);
	if ((cmd[0][0] == '<' || cmd[0][0] == '>' || cmd[0][0] == '|') && !cmd[1])
		return (g_exit_status = 1 << 8, dprintf(2, "21sh: parse error\n"),
			free_args(cmd), 0);
	cmd_count = count_args(cmd);
	if (ft_count_commands(cmd, cmd_count) == 1)
	{
		ft_change_env(*env, "_", cmd[cmd_count - 1]);
		exit_c = execute_ft_cmds(cmd, env);
		if (exit_c == 1)
			return (free_args(cmd), 0);
		else if (exit_c == 2)
			return (free_args(cmd), 1);
	}
	ft_parse_pipeline(cmd, cmd_count, *env, free_value);
	free_args(cmd);
	return (0);
}

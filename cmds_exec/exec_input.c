/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:50:52 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/24 19:06:39 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Execute a built-in command.
 * @param cmd The array of command arguments.
 * @param env The environment variables.
 * @return 1 if the command is recognized and executed, 0 otherwise.
 */
int	execute_ft_cmds(char **cmd, char ***env, int *exit_code)
{
	char	*current_pwd;

	if (ft_cd(cmd, env, exit_code))
		return (1);
	if (!ft_strncmp(cmd[0], "unset", 6))
		return (ft_unset(*env, cmd, exit_code), 1);
	if (!ft_strncmp(cmd[0], "pwd", 4))
	{
		current_pwd = ft_getenv(*env, "PWD");
		if (!current_pwd)
		{
			current_pwd = ft_get_current_dir();
			ft_dprintf(1, "%s\n", current_pwd);
			if (current_pwd)
				free(current_pwd);
		}
		else
			ft_dprintf(1, "%s\n", current_pwd);
		*exit_code = 0;
		return (1);
	}
	if (!ft_strncmp(cmd[0], "echo", 5))
		return (ft_echo(cmd, exit_code), 1);
	if (!ft_strncmp(cmd[0], "env", 4))
		return (ft_show_env(*env, exit_code), 1);
	if (!ft_strncmp(cmd[0], "export", 7))
		return (execute_ft_cmds_export(cmd, env, exit_code), 1);
	if (!ft_strncmp(cmd[0], "exit", 5))
		return (2);
	return (0);
}

int	syntax_error(char **cmd, int *exit_code)
{
	int	i;

	i = 0;
	while (cmd[i + 1])
	{
		if (cmd[i][0] == '<' && (cmd[i + 1][0] == '<' || cmd[i + 1][0] == '>'
			|| !cmd[i + 1]))
			return (*exit_code = 2 << 8,
				ft_dprintf(2, "21sh: syntax error near unexpected token\n"), 1);
		if (cmd[i][0] == '>' && (cmd[i + 1][0] == '<' || cmd[i + 1][0] == '>'
			|| !cmd[i + 1]))
			return (*exit_code = 2 << 8,
				ft_dprintf(2, "21sh: syntax error near unexpected token\n"), 1);
		if (cmd[i][0] == '|' && (cmd[i + 1][0] == '|' || !cmd[i + 1]))
			return (*exit_code = 2 << 8,
				ft_dprintf(2, "21sh: syntax error near unexpected token\n"), 1);
		i++;
	}
	if (cmd[i] && (cmd[i][0] == '<' || cmd[i][0] == '>' || cmd[i][0] == '|')
		&& !cmd[i + 1])
		return (*exit_code = 2 << 8,
			ft_dprintf(2, "21sh: syntax error near unexpected token\n"), 1);
	return (0);
}

/**
 * Execute a single command line.
 * @param env The environment variables.
 * @param input The command line to execute.
 * Processes input, splits into commands, executes.
 */
int	execute_input(char ***env, char *input, t_free *free_value, int *exit_code)
{
	char	**cmd;
	int		cmd_count;
	int		exit_c;

	cmd = parse_input(input, *env, exit_code);
	free_value->env = env;
	free_value->parsed_input = cmd;
	if (cmd == NULL || cmd[0] == NULL)
		return (0);
	if (syntax_error(cmd, exit_code))
		return (free_args(cmd), 0);
	cmd_count = count_args(cmd);
	if (ft_count_commands(cmd, cmd_count) == 1)
	{
		ft_change_env(*env, "_", cmd[cmd_count - 1]);
		exit_c = execute_ft_cmds(cmd, env, exit_code);
		if (exit_c == 1)
			return (free_args(cmd), 0);
		else if (exit_c == 2)
			return (free_args(cmd), 1);
	}
	ft_parse_pipeline(cmd, cmd_count, *env, free_value);
	free_args(cmd);
	return (0);
}

int	handle_sigint(t_cmd	*cmds)
{
	if (g_signal == SIGINT)
	{
		if (cmds->args)
			free(cmds->args);
		free(cmds);
		return (1);
	}
	return (0);
}

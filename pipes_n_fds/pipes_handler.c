/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 03:28:55 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/24 22:46:58 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Counts the number of command arguments in the given tokens.
 * @param tokens Array of string tokens representing the commands and arguments.
 * @param num_tokens Number of tokens in the tokens array.
 * @param start Starting index in the tokens array for the command.
 * @return The number of command arguments in the tokens array.
 */
int	ft_count_args(char **tokens, int num_tokens, int start)
{
	int	i;
	int	count;

	i = start;
	count = 0;
	while (i < num_tokens && ft_strcmp(tokens[i], "|") != 0)
	{
		if (ft_strcmp(tokens[i], "<") == 0 || ft_strcmp(tokens[i], ">") == 0
			||ft_strcmp(tokens[i], ">>") == 0
			|| ft_strcmp(tokens[i], "<<") == 0)
			i += 2;
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}

/**
 * @brief Parse tokens to fill t_cmd args.
 * @param cmd Pointer to t_cmd to fill.
 * @param tokens Array of command and argument tokens.
 * @param num_tokens Size of tokens array.
 * @param start Start index in tokens for command.
 * @return Index of next token or num_tokens if last.
 */
int	ft_fill_args(t_cmd *cmd, char **tokens, int num_tokens, int start)
{
	int	arg_index;
	int	i;

	arg_index = 0;
	i = start;
	while (i < num_tokens && ft_strcmp(tokens[i], "|") != 0)
	{
		if (ft_strcmp(tokens[i], "<") == 0 || ft_strcmp(tokens[i], ">") == 0
			|| ft_strcmp(tokens[i], ">>") == 0
			|| ft_strcmp(tokens[i], "<<") == 0)
		{
			ft_parse_redirection(cmd, tokens, &i);
			i++;
		}
		else
		{
			cmd->args[arg_index++] = tokens[i];
			i++;
		}
	}
	cmd->args[arg_index] = NULL;
	return (i);
}

/**
 * @brief Closes non-standard file descriptors for a command.
 * @param cmd The command to cleanup.
 */
void	cleanup_fds(t_cmd *cmd)
{
	if (cmd->input_fd != STDIN_FILENO && cmd->input_fd != -1)
		close(cmd->input_fd);
	if (cmd->output_fd != STDOUT_FILENO && cmd->output_fd != -1)
		close(cmd->output_fd);
}

void	cleanup_pipeline(t_cmd *cmds, int count, int safe_stdin, int *exit_code)
{
	int	i;

	(void)exit_code;
	i = -1;
	while (++i < count)
		free(cmds[i].args);
	free(cmds);
	dup2(safe_stdin, STDIN_FILENO);
	close(safe_stdin);
}

/**
 * @brief Parse and execute a pipeline of commands.
 * @param tokens Array of command and argument tokens.
 * @param num_tokens Size of tokens array.
 * @param env Environment variables.
 */
void	ft_parse_pipeline(char **tokens, int num_tokens, char **env,
	t_free *free_value)
{
	t_cmd			*cmds;
	int				count;
	int				safe_stdin;
	t_pipeline_ctx	ctx;
	int				i;

	i = -1;
	count = ft_count_commands(tokens, num_tokens);
	safe_stdin = dup(STDIN_FILENO);
	cmds = ft_parse_commands(tokens, num_tokens, env, free_value->exit_code);
	if (cmds_null_case(cmds, safe_stdin))
		return ;
	ctx.cmds = cmds;
	ctx.count = count;
	ctx.env = env;
	ctx.prev_pipe = STDIN_FILENO;
	while (++i < ctx.count)
		execute_command(&ctx, i, free_value, safe_stdin);
	i = -1;
	while (++i < ctx.count)
		wait_ignore(ctx.cmds[i].pid, free_value->exit_code);
	cleanup_pipeline(ctx.cmds, ctx.count, safe_stdin, free_value->exit_code);
}

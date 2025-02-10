/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 03:28:55 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/11 00:03:56 by hdelacou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
 * @brief Set up a pipe for a command in a pipeline context.
 * @param ctx Pipeline context.
 * @param index Index of the command to be piped.
 * @param pipe_fds Array of two file descriptors for the pipe.
 */
void	setup_pipe(t_pipeline_ctx *ctx, int index, int pipe_fds[2])
{
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	ctx->cmds[index].output_fd = pipe_fds[1];
	ctx->cmds[index + 1].input_fd = pipe_fds[0];
}

/**
 * @brief Closes non-standard file descriptors for a command.
 * @param cmd The command to cleanup.
 */
void	cleanup_fds(t_cmd *cmd)
{
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	if (cmd->output_fd != STDOUT_FILENO)
		close(cmd->output_fd);
}

/**
 * @brief Parse and execute a pipeline of commands.
 * @param tokens Array of command and argument tokens.
 * @param num_tokens Size of tokens array.
 * @param env Environment variables.
 * @param exit_status Pointer to store exit status of last command.
 */
void	ft_parse_pipeline(char **tokens, int num_tokens, char **env,
	int *exit_status)
{
	t_cmd			*cmds;
	int				count;
	t_pipeline_ctx	ctx;
	int				i;

	count = ft_count_commands(tokens, num_tokens);
	cmds = ft_parse_commands(tokens, num_tokens);
	ctx.cmds = cmds;
	ctx.count = count;
	ctx.env = env;
	ctx.prev_pipe = STDIN_FILENO;
	i = 0;
	while (i < ctx.count)
		execute_command(&ctx, i++);
	i = -1;
	while (++i < ctx.count)
		waitpid(-1, exit_status, 0);
	i = 0;
	while (i < count)
		free(cmds[i++].args);
	free(cmds);
}

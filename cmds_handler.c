/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 00:50:50 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/15 05:58:03 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Executes a command with redirection and built-in support.
 * Handles redirection, tries to execute as built-in, then searches in PATH.
 * If execution fails, prints an error message and exits with status 127.
 * @param cmd t_cmd structure with command and arguments.
 * @param env Environment variables array.
 */
void	ft_execute(t_cmd *cmd, char **env)
{
	char	*actual_cmd;

	actual_cmd = NULL;
	if (cmd->input_fd != STDIN_FILENO)
	{
		dup2(cmd->input_fd, STDIN_FILENO);
		close(cmd->input_fd);
	}
	if (cmd->output_fd != STDOUT_FILENO)
	{
		dup2(cmd->output_fd, STDOUT_FILENO);
		close(cmd->output_fd);
	}
	if (cmd->args && cmd->args[0])
	{
		if (execute_ft_cmds(cmd->args, &env))
			exit(0);
		else
			actual_cmd = get_cmd_path(cmd->args[0], ft_getenv(env, "PATH"));
	}
	if (actual_cmd)
		execve(actual_cmd, cmd->args, env);
	perror("21sh");
	exit(127);
}

/**
 * @brief Counts the number of commands in the given tokens.
 * Counts commands in tokens by counting "|" tokens.
 * @param tokens Array of string tokens representing the commands and arguments.
 * @param num_tokens Number of tokens in the tokens array.
 * @return The number of commands in the tokens array.
 */
int	ft_count_commands(char **tokens, int num_tokens)
{
	int	i;
	int	count;

	i = 0;
	count = 1;
	while (i < num_tokens)
	{
		if (ft_strcmp(tokens[i], "|") == 0)
			count++;
		i++;
	}
	return (count);
}

/**
 * @brief Process one command by filling the args field of the given t_cmd.
 * Processes one command by filling the args field of the given t_cmd.
 * @param tokens Array of string tokens representing the commands and arguments.
 * @param num_tokens Number of tokens in the tokens array.
 * @param start Starting index in the tokens array for the command.
 * @param cmd Pointer to a t_cmd structure to be filled.
 * @return The index of the next token after the command, or num_tokens if the
 * command is the last one.
 */
int	ft_process_one_command(char **tokens, int num_tokens, int start,
		t_cmd *cmd)
{
	int	arg_count;
	int	i;

	arg_count = ft_count_args(tokens, num_tokens, start);
	cmd->args = malloc((arg_count + 1) * sizeof(char *));
	if (!cmd->args)
		exit(EXIT_FAILURE);
	i = ft_fill_args(cmd, tokens, num_tokens, start);
	if (i < num_tokens && ft_strcmp(tokens[i], "|") == 0)
		i++;
	return (i);
}

/**
 * @brief Parses the given tokens into an array of command structures.
 * Parses tokens into an array of command structures.
 * @param tokens Array of string tokens representing the commands and arguments.
 * @param num_tokens Number of tokens in the tokens array.
 * @return Pointer to an array of t_cmd structures representing parsed commands.
 */
t_cmd	*ft_parse_commands(char **tokens, int num_tokens)
{
	t_cmd	*cmds;
	int		cmd_count;
	int		cmd_index;
	int		i;

	cmd_count = ft_count_commands(tokens, num_tokens);
	cmds = malloc(cmd_count * sizeof(t_cmd));
	if (!cmds)
		exit(EXIT_FAILURE);
	i = 0;
	cmd_index = 0;
	while (i < num_tokens && cmd_index < cmd_count)
	{
		cmds[cmd_index].input_fd = STDIN_FILENO;
		cmds[cmd_index].output_fd = STDOUT_FILENO;
		i = ft_process_one_command(tokens, num_tokens, i, &cmds[cmd_index]);
		cmd_index++;
	}
	return (cmds);
}

/**
 * Execute a command in a pipeline context.
 * Sets up pipes, forks, executes the command and handles FDs.
 * @param ctx: Pipeline context.
 * @param index: Index of the command to be executed.
 */
void	execute_command(t_pipeline_ctx *ctx, int index)
{
	int		pipe_fds[2];
	pid_t	pid;

	if (index < ctx->count - 1)
		setup_pipe(ctx, index, pipe_fds);
	pid = fork();
	if (pid == 0)
	{
		if (index > 0)
			ctx->cmds[index].input_fd = ctx->prev_pipe;
		ft_execute(&ctx->cmds[index], ctx->env);
	}
	else if (pid < 0)
		exit(EXIT_FAILURE);
	if (index > 0)
		close(ctx->prev_pipe);
	if (index < ctx->count - 1)
	{
		close(pipe_fds[1]);
		ctx->prev_pipe = pipe_fds[0];
	}
	cleanup_fds(&ctx->cmds[index]);
}

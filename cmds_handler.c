/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 00:50:50 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/20 19:28:05 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_before_exit(t_pipeline_ctx *ctx, t_free *free_value, int exit_code)
{
	int i;

	i = 0;
	free_args(free_value->parsed_input);
	free_args(*(free_value->env));
	free(free_value->relative_path);
	while (i < ctx->count)
	{
		free(ctx->cmds[i].args);
		if (ctx->cmds[i].input_fd != STDIN_FILENO && ctx->cmds[i].input_fd != -1)
			close(ctx->cmds[i].input_fd);
		if (ctx->cmds[i].output_fd != STDOUT_FILENO && ctx->cmds[i].output_fd != -1)
			close(ctx->cmds[i].output_fd);
		if (ctx->prev_pipe != STDIN_FILENO && ctx->prev_pipe != -1)
			close(ctx->prev_pipe);
		i++;
	}
	free(ctx->cmds);
	exit(exit_code);
}

/**
 * @brief Executes a command with redirection and built-in support.
 * Handles redirection, tries to execute as built-in, then searches in PATH.
 * If execution fails, prints an error message and exits with status 127.
 * @param cmd t_cmd structure with command and arguments.
 * @param env Environment variables array.
 */
void	ft_execute(t_pipeline_ctx *ctx, t_free *free_value, int index, int pipe_fds[2])
{
	char	*actual_cmd;
	int		i;

	(void)pipe_fds;
	i = 0;
	actual_cmd = NULL;
	if (ctx->cmds[index].input_fd != STDIN_FILENO && ctx->cmds[index].input_fd != -1)
	{
		dup2(ctx->cmds[index].input_fd, STDIN_FILENO);
		close(ctx->cmds[index].input_fd);
	}
	if (ctx->cmds[index].output_fd != STDOUT_FILENO && ctx->cmds[index].output_fd != -1)
	{
		dup2(ctx->cmds[index].output_fd, STDOUT_FILENO);
		close(ctx->cmds[index].output_fd);
	}
	if (ctx->cmds[index].args && ctx->cmds[index].args[0])
	{
		if (execute_ft_cmds(ctx->cmds[index].args, &ctx->env))
			free_before_exit(ctx, free_value, 0);
		else
			actual_cmd = get_cmd_path(ctx->cmds[index].args[0], ft_getenv(ctx->env, "PATH"));
	}
	else
		free_before_exit(ctx, free_value, 0);
	if (actual_cmd)
		execve(actual_cmd, ctx->cmds[index].args, ctx->env);
	ft_dprintf(2, "21sh: command not found : %s\n", ctx->cmds[index].args[0]);
	free_before_exit(ctx, free_value, 127);
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
		cmds[cmd_index].count = cmd_count;
		i = ft_process_one_command(tokens, num_tokens, i, &cmds[cmd_index]);
		if (ft_strcmp(tokens[i - 1], "|") == 0 && !tokens[i])
			return (ft_dprintf(2, "21sh: empty pipe\n"), NULL);
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
void	execute_command(t_pipeline_ctx *ctx, int index, t_free *free_value)
{
	int		pipe_fds[2];
	pid_t	pid;
	int		use_pipe;

	use_pipe = 0;
    if (index < ctx->count - 1 && ctx->cmds[index].output_fd == STDOUT_FILENO)
    {
        if (pipe(pipe_fds) == -1)
            exit(EXIT_FAILURE);
        ctx->cmds[index].output_fd = pipe_fds[1];
        use_pipe = 1;
    }
    pid = fork();
    ctx->cmds[index].pid = pid;
    if (pid == 0)
    {
        if (index > 0 && ctx->cmds[index].input_fd == STDIN_FILENO)
            ctx->cmds[index].input_fd = ctx->prev_pipe;
        if (use_pipe)
            close(pipe_fds[0]);
		for (int i = 0; i < ctx->count; i++)
        {
			if (i != index && i > index - 1)
			{
				if (ctx->cmds[i].input_fd != STDIN_FILENO && ctx->cmds[i].input_fd != -1)
					close(ctx->cmds[i].input_fd);
				if (ctx->cmds[i].output_fd != STDOUT_FILENO && ctx->cmds[i].output_fd != -1)
					close(ctx->cmds[i].output_fd);
			}
        }
        ft_execute(ctx, free_value, index, pipe_fds);
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
        exit(EXIT_FAILURE);
    if (index > 0 && ctx->prev_pipe != STDIN_FILENO && ctx->prev_pipe != -1)
        close(ctx->prev_pipe);
    if (use_pipe)
    {
        close(pipe_fds[1]);
        ctx->prev_pipe = pipe_fds[0];
    }
	cleanup_fds(&ctx->cmds[index]);
}

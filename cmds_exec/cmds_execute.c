/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_execute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 00:16:52 by hdelacou          #+#    #+#             */
/*   Updated: 2025/02/23 06:23:48 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Close all file descriptors in a pipeline context,
 * except for the one at a given command index.
 * @param ctx Pipeline context.
 * @param index Index of the command whose
 * file descriptors should not be closed.
 */
static void	close_fds(t_pipeline_ctx *ctx, int index)
{
	int	i;

	i = index + 1;
	while (i < ctx->count)
	{
		if (index < ctx->count - 1)
		{
			if (ctx->cmds[i].input_fd != STDIN_FILENO
				&& ctx->cmds[i].input_fd != -1)
				close(ctx->cmds[i].input_fd);
			if (ctx->cmds[i].output_fd != STDOUT_FILENO
				&& ctx->cmds[i].output_fd != -1)
				close(ctx->cmds[i].output_fd);
		}
		i++;
	}
}

void	handle_pid(t_pipeline_ctx *ctx, t_free *free_value,
	t_pid_struct *new_pid, int safe_stdin)
{
	if (new_pid->pid == 0)
	{
		close(safe_stdin);
		if (ctx->cmds[new_pid->index].input_fd != STDIN_FILENO
			&& ctx->cmds[new_pid->index].input_fd != -1)
			close(ctx->prev_pipe);
		close_fds(ctx, new_pid->index);
		if (new_pid->index > 0
			&& ctx->cmds[new_pid->index].input_fd == STDIN_FILENO)
			ctx->cmds[new_pid->index].input_fd = ctx->prev_pipe;
		if (new_pid->use_pipe)
			close(new_pid->pipe_fds[0]);
		ft_execute(ctx, free_value, new_pid);
		exit(EXIT_FAILURE);
	}
	else if (new_pid->pid < 0)
		exit(EXIT_FAILURE);
	if (new_pid->use_pipe && ctx->cmds[new_pid->index].input_fd == -1)
		close(new_pid->pipe_fds[0]);
	wait_ignore(new_pid->pid, free_value->exit_code);
}

/**
 * Execute a command in a pipeline context.
 * Sets up pipes, forks, executes the command and handles FDs.
 * @param ctx: Pipeline context.
 * @param index: Index of the command to be executed.
 */
void	execute_command(t_pipeline_ctx *ctx, int index, t_free *free_value,
	int safe_stdin)
{
	int				pipe_fds[2];
	pid_t			pid;
	int				use_pipe;
	t_pid_struct	*new_pid;

	use_pipe = 0;
	setup_signals(0);
	if (index < ctx->count - 1 && ctx->cmds[index].output_fd == STDOUT_FILENO)
	{
		if (pipe(pipe_fds) == -1)
			exit(EXIT_FAILURE);
		ctx->cmds[index].output_fd = pipe_fds[1];
		use_pipe = 1;
	}
	pid = fork();
	ctx->cmds[index].pid = pid;
	new_pid = handle_new_pid(index, pipe_fds, use_pipe, pid);
	handle_pid(ctx, free_value, new_pid, safe_stdin);
	free(new_pid);
	if (index > 0 && ctx->prev_pipe != STDIN_FILENO && ctx->prev_pipe != -1)
		close(ctx->prev_pipe);
	if (use_pipe)
		ctx->prev_pipe = pipe_fds[0];
	cleanup_fds(&ctx->cmds[index]);
}

void	handle_dup2(t_pipeline_ctx *ctx, t_free *free_value,
	t_pid_struct *new_pid)
{
	if (ctx->cmds[new_pid->index].input_fd == -1
		|| ctx->cmds[new_pid->index].output_fd == -1)
	{
		if (ctx->cmds[new_pid->index].output_fd != STDOUT_FILENO
			&& ctx->cmds[new_pid->index].output_fd != -1)
			close(ctx->cmds[new_pid->index].output_fd);
		if (ctx->cmds[new_pid->index].input_fd != STDIN_FILENO
			&& ctx->cmds[new_pid->index].input_fd != -1)
			close(ctx->cmds[new_pid->index].input_fd);
		free_before_exit(ctx, free_value, 1, new_pid);
	}
	if (ctx->cmds[new_pid->index].input_fd != STDIN_FILENO
		&& ctx->cmds[new_pid->index].input_fd != -1)
	{
		dup2(ctx->cmds[new_pid->index].input_fd, STDIN_FILENO);
		close(ctx->cmds[new_pid->index].input_fd);
	}
	if (ctx->cmds[new_pid->index].output_fd != STDOUT_FILENO
		&& ctx->cmds[new_pid->index].output_fd != -1)
	{
		dup2(ctx->cmds[new_pid->index].output_fd, STDOUT_FILENO);
		close(ctx->cmds[new_pid->index].output_fd);
	}
}

/**
 * @brief Executes a command with redirection and built-in support.
 * Handles redirection, tries to execute as built-in, then searches in PATH.
 * If execution fails, prints an error message and exits with status 127.
 * @param cmd t_cmd structure with command and arguments.
 * @param env Environment variables array.
 */
void	ft_execute(t_pipeline_ctx *ctx, t_free *free_value,
	t_pid_struct *new_pid)
{
	char	*actual_cmd;

	actual_cmd = NULL;
	handle_dup2(ctx, free_value, new_pid);
	if (ctx->cmds[new_pid->index].args && ctx->cmds[new_pid->index].args[0])
	{
		if (execute_ft_cmds(ctx->cmds[new_pid->index].args, &ctx->env,
				free_value->exit_code))
			free_before_exit(ctx, free_value, 0, new_pid);
		else
			actual_cmd = get_cmd_path(ctx->cmds[new_pid->index].args[0],
					ft_getenv(ctx->env, "PATH"));
	}
	else
		free_before_exit(ctx, free_value, 0, new_pid);
	if (actual_cmd)
		execve(actual_cmd, ctx->cmds[new_pid->index].args, ctx->env);
	print_ifpath_ornot(ctx->cmds[new_pid->index].args[0]);
	free(actual_cmd);
	free_before_exit(ctx, free_value, 127, new_pid);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 00:50:50 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/07 22:53:00 by hdelacou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

void	ft_execute(t_cmd *cmd, char **env)
{
	char	*actual_cmd;
	pid_t	pid;
	int		status;

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
	actual_cmd = get_cmd_path(cmd->args[0], ft_getenv(env, "PATH"));
	pid = fork();
	if (pid == 0)
	{
		if (execve(actual_cmd, cmd->args, env) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
	}
	else
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
}

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

int	ft_process_one_command(char **tokens, int num_tokens, int start, t_cmd *cmd)
{
	int	arg_count;
	int	i;

	arg_count = ft_count_args(tokens, num_tokens, start);
	cmd->args = malloc((arg_count + 1) * sizeof(char *));
	if (!cmd->args)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	i = ft_fill_args(cmd, tokens, num_tokens, start);
	if (i < num_tokens && ft_strcmp(tokens[i], "|") == 0)
		i++;
	return (i);
}

t_cmd	*ft_parse_commands(char **tokens, int num_tokens)
{
	t_cmd	*cmds;
	int		cmd_count;
	int		cmd_index;
	int		i;

	cmd_count = ft_count_commands(tokens, num_tokens);
	cmds = malloc(cmd_count * sizeof(t_cmd));
	if (!cmds)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
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
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (index > 0)
		close(ctx->prev_pipe);
	if (index < ctx->count - 1)
	{
		close(pipe_fds[1]);
		ctx->prev_pipe = pipe_fds[0];
	}
	cleanup_fds(&ctx->cmds[index]);
}

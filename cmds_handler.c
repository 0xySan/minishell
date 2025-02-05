/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 00:50:50 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/05 03:34:27 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execute(t_cmd *cmd, char **env)
{
	char	*actural_cmd;

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
	actural_cmd = get_cmd_path(cmd->args[0], ft_getenv(env, "PATH"));
	execvp(cmd->args[0], cmd->args);
	perror("execvp");
	exit(EXIT_FAILURE);
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

int	ft_process_one_command(char **tokens, int num_tokens, int start,
		t_cmd *cmd)
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 03:28:55 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/05 03:35:16 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	setup_pipe(t_pipeline_ctx *ctx, int index, int pipe_fds[2])
{
	if (pipe(pipe_fds) < 0)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	ctx->cmds[index].output_fd = pipe_fds[1];
	ctx->cmds[index + 1].input_fd = pipe_fds[0];
}

void	cleanup_fds(t_cmd *cmd)
{
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	if (cmd->output_fd != STDOUT_FILENO)
		close(cmd->output_fd);
}

void	ft_parse_pipeline(char **tokens, int num_tokens, char **env)
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
		wait(NULL);
	while (i < count)
		free(cmds[i++].args);
	free(cmds);
}

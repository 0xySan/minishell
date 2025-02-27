/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 03:28:55 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/27 17:14:24 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	cleanup_fds(t_cmd *cmd)
{
	if (cmd->input_fd != STDIN_FILENO && cmd->input_fd != -1)
		close(cmd->input_fd);
	if (cmd->output_fd != STDOUT_FILENO && cmd->output_fd != -1)
		close(cmd->output_fd);
}

void	cleanup_pipeline(t_cmd *cmds, int count, int *exit_code)
{
	int	i;

	(void)exit_code;
	i = -1;
	while (++i < count)
		free(cmds[i].args);
	free(cmds);
}

void	ft_parse_pipeline(char **tokens, int num_tokens, char **env,
	t_free *free_value)
{
	t_cmd			*cmds;
	int				count;
	t_pipeline_ctx	ctx;
	int				i;
	int				ret;

	count = ft_count_commands(tokens, num_tokens);
	ret = handle_here_doc_tokens(tokens);
	if (ret == -1)
		return ;
	cmds = ft_parse_commands(tokens, num_tokens, env, free_value->exit_code);
	ctx.cmds = cmds;
	ctx.count = count;
	ctx.env = env;
	ctx.prev_pipe = STDIN_FILENO;
	i = -1;
	while (++i < ctx.count)
		execute_command(&ctx, i, free_value);
	i = -1;
	while (++i < ctx.count)
		wait_ignore(ctx.cmds[i].pid, free_value->exit_code);
	cleanup_pipeline(ctx.cmds, ctx.count, free_value->exit_code);
	close_here_doc(ret);
}

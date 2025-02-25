/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 00:50:50 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/25 15:34:12 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_before_exit(t_pipeline_ctx *ctx, t_free *free_value, int exit_code,
	t_pid_struct *new_pid)
{
	int	i;

	i = 0;
	free_args(free_value->parsed_input);
	free_args(*(free_value->env));
	free(free_value->relative_path);
	while (i < ctx->count)
	{
		free(ctx->cmds[i].args);
		i++;
	}
	free(new_pid);
	free(ctx->cmds);
	exit(exit_code);
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
	cmd->args = malloc((arg_count * 2 + 1) * sizeof(char *));
	if (!cmd->args)
		exit(EXIT_FAILURE);
	i = ft_fill_args(cmd, tokens, num_tokens, start);
	if (i < num_tokens && ft_strcmp(tokens[i], "|") == 0)
		i++;
	return (i);
}

t_pid_struct	*handle_new_pid(int index, int *pipe_fds, int use_pipe,
	pid_t pid)
{
	t_pid_struct	*new_pid;

	new_pid = malloc(sizeof(t_pid_struct));
	if (!new_pid)
		exit(EXIT_FAILURE);
	new_pid->index = index;
	new_pid->pipe_fds[0] = pipe_fds[0];
	new_pid->pipe_fds[1] = pipe_fds[1];
	new_pid->use_pipe = use_pipe;
	new_pid->pid = pid;
	return (new_pid);
}

t_cmd	*ft_parse_commands(char **tokens, int num_tokens, char **env,
	int *exit_code)
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
	cmd_index = -1;
	while (i < num_tokens && ++cmd_index < cmd_count)
	{
		if (handle_sigint(cmds))
			return (NULL);
		cmds[cmd_index].input_fd = STDIN_FILENO;
		cmds[cmd_index].output_fd = STDOUT_FILENO;
		cmds[cmd_index].count = cmd_count;
		cmds[cmd_index].env = env;
		cmds[cmd_index].exit_code = exit_code;
		i = ft_process_one_command(tokens, num_tokens, i, &cmds[cmd_index]);
		if (ft_strcmp(tokens[i - 1], "|") == 0 && !tokens[i])
			return (ft_dprintf(2, "21sh: empty pipe\n"), NULL);
	}
	return (cmds);
}

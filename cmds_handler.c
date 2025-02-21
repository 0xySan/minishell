/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 00:50:50 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/22 00:32:02 by hdelacou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees all allocated memory and exits with a given status code.
 * @param ctx Pipeline context with allocated memory for the commands.
 * @param free_value Structure containing pointers to allocated memory to free.
 * @param exit_code Status code to exit with.
 */
void	free_before_exit(t_pipeline_ctx *ctx, t_free *free_value, int exit_code)
{
	int	i;

	i = 0;
	free_args(free_value->parsed_input);
	free_args(*(free_value->env));
	free(free_value->relative_path);
	while (i < ctx->count)
	{
		free(ctx->cmds[i].args);
		if (ctx->cmds[i].input_fd != STDIN_FILENO
			&& ctx->cmds[i].input_fd != -1)
			close(ctx->cmds[i].input_fd);
		if (ctx->cmds[i].output_fd != STDOUT_FILENO
			&& ctx->cmds[i].output_fd != -1)
			close(ctx->cmds[i].output_fd);
		if (ctx->prev_pipe != STDIN_FILENO && ctx->prev_pipe != -1)
			close(ctx->prev_pipe);
		i++;
	}
	free(ctx->cmds);
	exit(exit_code);
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
int	ft_process_one_command(char **tokens, int num_tokens, int start, t_cmd *cmd)
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

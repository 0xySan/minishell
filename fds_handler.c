/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 02:54:35 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/19 14:48:56 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Opens a file, or /dev/null on failure.
 * @param path The path of the file to open.
 * @param flags The flags used to open the file (e.g., O_RDONLY).
 * @param mode The mode to use if a new file is created.
 * @return File descriptor on success, or /dev/null on failure.
 */
static int	open_file(char *path, int flags, int mode)
{
	int	fd;

	fd = open(path, flags, mode);
	if (fd < 0)
	{
		ft_dprintf(2, "21sh: no such file or directory: %s\n", path);
		fd = -1;
	}
	return (fd);
}

/**
 * @brief Reads lines until 'delimiter' is entered, returning a pipe's read end.
 * @param delimiter The string that signifies the end of input.
 * @return File descriptor for the read end of the pipe containing the input.
 */
static int	handle_here_doc(t_cmd *cmd, char *delimiter)
{
	char	*line;
	int		pipe_fd[2];

	pipe(pipe_fd);
	if (!delimiter && cmd->input_fd != -1)
		return (cmd->input_fd = pipe_fd[0], close(pipe_fd[1]));
	while (1)
	{
		line = readline("> ");
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_dprintf(pipe_fd[1], "%s\n", line);
		free(line);
	}
	close(pipe_fd[1]);
	if (cmd->input_fd == -1)
		return (close(pipe_fd[0]));
	cmd->input_fd = pipe_fd[0];
	return (0);
}

/**
 * @brief Parses redirections in a command.
 * @param cmd The command to which to assign the file descriptors.
 * @param tokens The array of string tokens representing the command.
 * @param i The index of the current token in the tokens array.
 */
void	ft_parse_redirection(t_cmd *cmd, char **tokens, int *i)
{
	if (ft_strcmp(tokens[*i], "<<") == 0)
		handle_here_doc(cmd, tokens[++(*i)]);
	if (cmd->input_fd == -1 || !tokens[(*i + 1)])
		return ;
	if (ft_strcmp(tokens[*i], "<") == 0)
	{
		if (cmd->input_fd != STDIN_FILENO)
			close(cmd->input_fd);
		cmd->input_fd = open_file(tokens[++(*i)], O_RDONLY, 0);
	}
	else if (ft_strcmp(tokens[*i], ">") == 0)
	{
		if (cmd->output_fd != STDOUT_FILENO)
			close(cmd->output_fd);
		cmd->output_fd = open_file(tokens[++(*i)],
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else if (ft_strcmp(tokens[*i], ">>") == 0)
	{
		if (cmd->output_fd != STDOUT_FILENO)
			close(cmd->output_fd);
		cmd->output_fd = open_file(tokens[++(*i)],
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
}

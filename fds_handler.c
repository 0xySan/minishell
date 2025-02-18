/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 02:54:35 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/18 02:56:35 by etaquet          ###   ########.fr       */
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
		fd = open("/dev/null", flags, mode);
	return (fd);
}

/**
 * @brief Reads lines until 'delimiter' is entered, returning a pipe's read end.
 * @param delimiter The string that signifies the end of input.
 * @return File descriptor for the read end of the pipe containing the input.
 */
static int	handle_here_doc(char *delimiter)
{
	char	*line;
	int		pipe_fd[2];

	pipe(pipe_fd);
	if (!delimiter)
		return (pipe_fd[0]);
	while (1)
	{
		line = readline("> ");
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

/**
 * @brief Parses redirections in a command.
 * @param cmd The command to which to assign the file descriptors.
 * @param tokens The array of string tokens representing the command.
 * @param i The index of the current token in the tokens array.
 */
void	ft_parse_redirection(t_cmd *cmd, char **tokens, int *i)
{
	if (ft_strcmp(tokens[*i], "<") == 0)
		cmd->input_fd = open_file(tokens[++(*i)], O_RDONLY, 0);
	else if (ft_strcmp(tokens[*i], ">") == 0)
		cmd->output_fd = open_file(tokens[++(*i)],
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (ft_strcmp(tokens[*i], ">>") == 0)
		cmd->output_fd = open_file(tokens[++(*i)],
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (ft_strcmp(tokens[*i], "<<") == 0)
		cmd->input_fd = handle_here_doc(tokens[++(*i)]);
}

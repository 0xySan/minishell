/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 02:54:35 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/06 01:36:01 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_file(char *path, int flags, int mode)
{
	int	fd;

	fd = open(path, flags, mode);
	if (fd < 0)
	{
		perror("open");
		exit(127);
		// have to move the exit after the pipe and make sure that nothing works before another pipe
	}
	return (fd);
}

static int	handle_here_doc(char *delimiter)
{
	char	*line;
	int		pipe_fd[2];

	pipe(pipe_fd);
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
	{
		++(*i);
		cmd->input_fd = handle_here_doc(tokens[*i]);
	}
}

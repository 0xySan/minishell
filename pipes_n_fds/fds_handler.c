/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 02:54:35 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/27 17:08:52 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_file(char *path, int flags, int mode)
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

int	process_null(char *line, char *delimiter)
{
	if (line == NULL)
	{
		if (g_signal != SIGINT)
			dprintf(2, "21sh: warning: %s `%s')\n",
				"here-document delimited by end-of-file (wanted", delimiter);
		else
			return (-1);
		return (0);
	}
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (0);
	}
	return (1);
}

int	handle_here_doc(char *delimiter, int fd)
{
	char	*line;
	int		ret;

	if (g_signal == SIGINT)
		return (-1);
	setup_signals(2);
	if (delimiter == NULL)
		return (1);
	while (1)
	{
		line = readline("> ");
		ret = process_null(line, delimiter);
		if (ret == 0)
			break ;
		if (ret == -1)
			return (-1);
		ft_dprintf(fd, "%s\n", line);
		free(line);
	}
	if (ret == -1)
		return (-1);
	return (0);
}

void	ft_parse_redirection(t_cmd *cmd, char **tokens, int *i)
{
	if (cmd->input_fd == -1 || !tokens[(*i + 1)])
		return ;
	if (ft_strcmp(tokens[*i], "<") == 0)
	{
		if (cmd->input_fd != STDIN_FILENO && cmd->input_fd != -1)
			close(cmd->input_fd);
		cmd->input_fd = open_file(tokens[++(*i)], O_RDONLY, 0);
	}
	else if (ft_strcmp(tokens[*i], ">") == 0)
	{
		if (cmd->output_fd != STDOUT_FILENO && cmd->output_fd != -1)
			close(cmd->output_fd);
		cmd->output_fd = open_file(tokens[++(*i)],
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else if (ft_strcmp(tokens[*i], ">>") == 0)
	{
		if (cmd->output_fd != STDOUT_FILENO && cmd->output_fd != -1)
			close(cmd->output_fd);
		cmd->output_fd = open_file(tokens[++(*i)],
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
}

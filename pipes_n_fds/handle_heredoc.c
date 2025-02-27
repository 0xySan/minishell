/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:14:01 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/27 17:15:59 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*create_here_doc_path(int index)
{
	char	*itoa_value;
	char	*path;

	itoa_value = ft_itoa(index);
	path = ft_strjoin("/tmp/.here_doc_21sh_", itoa_value);
	free(itoa_value);
	return (path);
}

int	execute_here_doc(char *tmp, char **tokens, int i)
{
	int		safe_stdin;
	int		fd;
	int		ret;

	safe_stdin = dup(STDIN_FILENO);
	fd = open(tmp, O_CREAT | O_RDWR | O_TRUNC, 0644);
	ret = handle_here_doc(tokens[i + 1], fd);
	close(fd);
	if (ret == 0)
	{
		free(tokens[i]);
		tokens[i] = ft_strdup("<");
		free(tokens[i + 1]);
		tokens[i + 1] = ft_strdup(tmp);
	}
	free(tmp);
	if (ret == -1)
	{
		dup2(safe_stdin, STDIN_FILENO);
		close(safe_stdin);
		return (-1);
	}
	close(safe_stdin);
	return (0);
}

int	process_single_here_doc(char **tokens, int i, int *j)
{
	char	*tmp;

	tmp = create_here_doc_path((*j)++);
	if (access(tmp, F_OK) == 0
		&& (access(tmp, R_OK) != 0 || access(tmp, W_OK) != 0))
	{
		free(tmp);
		return (1);
	}
	return (execute_here_doc(tmp, tokens, i));
}

void	close_here_doc(int index)
{
	char	*itoa_value;
	char	*path;
	int		i;

	i = 0;
	while (i < index)
	{
		itoa_value = ft_itoa(i);
		path = ft_strjoin("/tmp/.here_doc_21sh_", itoa_value);
		if (access(path, F_OK) == 0)
			unlink(path);
		free(itoa_value);
		free(path);
		i++;
	}
}

int	handle_here_doc_tokens(char **tokens)
{
	int	i;
	int	j;
	int	ret;

	i = 0;
	j = 0;
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], "<<") == 0)
		{
			ret = process_single_here_doc(tokens, i, &j);
			if (ret == -1)
				return (close_here_doc(j), -1);
			if (ret == 1)
				continue ;
		}
		i++;
	}
	return (j);
}

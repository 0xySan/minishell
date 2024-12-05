/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:18:53 by etaquet           #+#    #+#             */
/*   Updated: 2024/12/05 18:49:37 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	sigint_handler(int sig)
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigquit_handler(int sig)
{
	exit(0);
}

void	get_relative_path(t_path *pwd)
{
	char	*r_value;
	int		m_len;

	m_len = 0;
	if (strncmp(pwd->tpath, "/home/etaquet/", 15))
		m_len = ft_strlen(ft_substr(pwd->tpath, 13, strlen(pwd->tpath))) + 4;
	else
	{
		pwd->rpath = pwd->tpath;
		return ;
	}
	if (m_len < 1)
		return ;
	r_value = malloc(m_len * sizeof(char));
	r_value[0] = '~';
	ft_strcat(r_value, ft_substr(pwd->tpath, 13, strlen(pwd->tpath)));
	ft_strcpy(pwd->rpath, r_value);
	free(r_value);
}

int	get_last_char_pos(char *str, char car)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	while (str[i] != car)
		i--;
	return (i);
}

void	get_new_relative_path(t_path *pwd, char *path)
{
	if (!ft_strncmp(".", path, 2))
		return ;
	if (!ft_strncmp("..", path, 3))
	{
		pwd->rpath = ft_substr(pwd->rpath, 0, get_last_char_pos(pwd->rpath, '/'));
		return ;
	}
	ft_strcat(pwd->rpath, "/");
	ft_strcat(pwd->rpath, path);
}

void	get_new_true_path(t_path *pwd, char *path)
{
	if (!ft_strncmp(".", path, 2))
		return ;
	if (!ft_strncmp("..", path, 3))
	{
		// if (ft_strncmp(pwd->tpath, "/home\0", ft_strlen(pwd->tpath)));
		// 	printf("%s\n", pwd->tpath);
		pwd->tpath = ft_substr(pwd->tpath, 0, get_last_char_pos(pwd->tpath, '/'));
		return ;
	}
	ft_strcat(pwd->tpath, "/");
	ft_strcat(pwd->tpath, path);
}

int	check_if_accessible(t_path *pwd, char *path)
{
	char	*nw_path;
	int		accessible;

	nw_path = malloc(ft_strlen(pwd->tpath) + ft_strlen(path) + 1);
	ft_strcpy(nw_path, pwd->tpath);
	ft_strcat(nw_path, "/");
	ft_strcat(nw_path, path);
	accessible = access(nw_path, F_OK);
	if (accessible != -1)
	{
		chdir(nw_path);
		get_new_relative_path(pwd, path);
		get_new_true_path(pwd, path);
	}
	free(nw_path);
	return (accessible);
}

int	read_lines(t_path *pwd, char **env)
{
	char	*input;
	char	*w_path;

	w_path = malloc(sizeof(pwd->rpath) + 2);
	ft_strcpy(w_path, pwd->rpath);
	ft_strcat(w_path, ": ");
	input = readline(w_path);
	free(w_path);
	if (input == NULL)
	{
		printf("Exiting 21sh...\n");
		free(input);
		return (1);
	}
	if (input)
	{
		if (check_if_accessible(pwd, input) == -1)
		{
			free(input);
			execve("/bin/ls", ft_split("/bin/ls", ' '), env);
		}
	}
	free(input);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_path	path;
	char	cwd[PATH_MAX];
	char	*nw_path;

	path.tpath = malloc(sizeof(char) * PATH_MAX);
	path.rpath = malloc(sizeof(char) * PATH_MAX);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd() error");
		exit(1);
	}
	path.tpath = cwd;
	get_relative_path(&path);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	while (1)
	{
		if (read_lines(&path, env))
			break ;
	}
	free(path.rpath);
	return (0);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:18:53 by etaquet           #+#    #+#             */
/*   Updated: 2024/12/05 20:03:40 by etaquet          ###   ########.fr       */
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

char	*fake_substr(char *s, unsigned int start, size_t len)
{
	char	*r_value;
	size_t	i;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (malloc(1));
	if (len > s_len - start)
		len = s_len - start;
	r_value = malloc(sizeof(char) * (len + 1));
	if (!r_value)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		r_value[i] = s[start + i];
		i++;
	}
	r_value[i] = '\0';
	return (r_value);
}

char	*get_relative_path(char *pwd)
{
    char	*home;
    size_t	home_len;
    size_t	pwd_len;

    home = getenv("HOME");
    if (!home)
        return (ft_strdup(pwd));
    home_len = ft_strlen(home);
    pwd_len = ft_strlen(pwd);
    if (home_len > pwd_len || ft_strncmp(home, pwd, home_len) != 0)
        return (ft_strdup(pwd));
    return (ft_strjoin("~", pwd + home_len));
}

int	check_if_accessible(char *cwd, char *path)
{
	char	*nw_path;
	int		accessible;

	nw_path = malloc(ft_strlen(cwd) + ft_strlen(path) + 3);
	ft_strcpy(nw_path, cwd);
	ft_strcat(nw_path, "/");
	ft_strcat(nw_path, path);
	accessible = access(nw_path, F_OK);
	if (accessible != -1)
		chdir(nw_path);
	free(nw_path);
	return (accessible);
}

int	read_lines(char *cwd, char **env)
{
	char	*input;
	char	*w_path;
	char	*r_path;

	r_path = get_relative_path(cwd);
	printf("\e[1m\x1B[31m");
	ft_strcat(r_path, ": \e[0m");
	input = readline(r_path);
	if (input == NULL)
	{
		printf("Exiting 21sh...\n");
		free(input);
		return (1);
	}
	if (input)
	{
		if (check_if_accessible(cwd, input) == -1)
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

	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	while (1)
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			perror("getcwd() error");
			exit(1);
		}
		if (read_lines(cwd, env))
			break ;
	}
	return (0);
}


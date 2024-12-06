/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:18:53 by etaquet           #+#    #+#             */
/*   Updated: 2024/12/06 14:32:49 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// to use it : gcc -o test test.c -lreadline -lhistory libft/libft.a

#include "test.h"

void	sigint_handler(int sig)
{
	printf("\e[1m\x1B[31m\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigquit_handler(int sig)
{
	printf("\e[1m\x1B[31m");
	rl_on_new_line();
	rl_redisplay();
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

	if (access(path, F_OK) != -1)
	{
		chdir(path);
		return (1);
	}
	nw_path = malloc(ft_strlen(cwd) + ft_strlen(path) + 2);
	ft_strcpy(nw_path, cwd);
	ft_strcat(nw_path, "/");
	ft_strcat(nw_path, path);
	accessible = access(nw_path, F_OK);
	if (accessible != -1)
		chdir(nw_path);
	free(nw_path);
	return (accessible);
}

int	ft_cd(char *cwd, char *path, char **cmd)
{
	if (!ft_strncmp(cmd[0], "cd", 3))
	{
		if (!cmd[1] || (cmd[1][0] == '~' && ft_strlen(cmd[1]) == 1))
			chdir(getenv("HOME"));
		else
			if (check_if_accessible(cwd, cmd[1]) == -1)
				printf("cd: no such file or directory: %s\n", cmd[1]);
		return (1);
	}
	return (0);
}

int	read_lines(char *cwd, char **env)
{
	char	*input;
	char	*r_path;
	char	**cmd;

	signal(SIGQUIT, sigquit_handler);
	r_path = get_relative_path(cwd);
	printf("\e[1m\x1B[31m");
	ft_strcat(r_path, ": \e[m");
	input = readline(r_path);
	if (input == 0 || !strncmp(input, "exit", ft_strlen(input)))
		return (printf("Exiting 21sh...\n"), free(input), 1);
	if (input)
	{
		add_history(input);
		cmd = ft_split(input, ' ');
		if (!ft_cd(cwd, input, cmd))
			execve(ft_strjoin("/bin/", cmd[0]), cmd, env);
	}
	free(input);
	free(r_path);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_path	path;
	char	cwd[PATH_MAX];

	signal(SIGINT, sigint_handler);
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


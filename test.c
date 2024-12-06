/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:18:53 by etaquet           #+#    #+#             */
/*   Updated: 2024/12/06 17:07:57 by etaquet          ###   ########.fr       */
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
	nw_path = malloc(ft_strlen(cwd) + ft_strlen(path) + 10000);
	ft_strcpy(nw_path, cwd);
	ft_strcat(nw_path, "/");
	ft_strcat(nw_path, path);
	accessible = access(nw_path, F_OK);
	if (accessible != -1)
		chdir(nw_path);
	free(nw_path);
	return (accessible);
}

int	check_if_raccessible(char *cwd, char *path)
{
	char	*nw_path;
	int		accessible;
	char	*home;
	char	*tmp;

	home = getenv("HOME");
	nw_path = malloc(ft_strlen(home) + ft_strlen(path) + 10000);
	ft_strcpy(nw_path, home);
	ft_strcat(nw_path, "/");
	tmp = ft_substr(path, 2, ft_strlen(path));
	ft_strcat(nw_path, tmp);
	accessible = access(nw_path, F_OK);
	if (accessible != -1)
		chdir(nw_path);
	free(nw_path);
	if (tmp)
		free(tmp);
	return (accessible);
}

int	ft_cd(char *cwd, char *path, char **cmd)
{
	char	*home;

	home = getenv("HOME");
	if (!ft_strncmp(cmd[0], "cd", 3))
	{
		if (!cmd[1] || (cmd[1][0] == '~' && ft_strlen(cmd[1]) == 1))
			chdir(home);
		else if (cmd[1][0] == '~' && ft_strlen(cmd[1]) > 1)
		{
			if (check_if_raccessible(cwd, cmd[1]) == -1)
				perror("cd");
		}
		else
			if (check_if_accessible(cwd, cmd[1]) == -1)
				perror("cd");
		return (1);
	}
	return (0);
}

void	child_process(char **cmd, char **envp, t_pidstruct	*pid)
{
	pid->pid[0] = fork();
	if (pid->pid[0] == -1)
		perror("Error: Fork failed.\n");
	if (pid->pid[0] == 0)
	{
		if (access(ft_strjoin("/bin/", cmd[0]), X_OK) != -1)
			execve(ft_strjoin("/bin/", cmd[0]), cmd, envp);
		else
			perror("21sh");
	}
}

int	read_lines(char *cwd, char **env)
{
	char		*input;
	char		*r_path;
	char		**cmd;
	t_pidstruct	pid;

	pid.pid = malloc(sizeof(pid_t) * 20);
	signal(SIGQUIT, sigquit_handler);
	r_path = get_relative_path(cwd);
	printf("\e[1m\x1B[31m");
	r_path = ft_strjoin(r_path, ": \e[m");
	input = readline(r_path);
	if (input == 0 || !strncmp(input, "exit", ft_strlen(input)))
		return (printf("Exiting 21sh...\n"), free(input), free(r_path), exit(0), 1);
	if (input)
	{
		add_history(input);
		cmd = ft_split(input, ' ');
		if (!ft_cd(cwd, input, cmd))
		{
			child_process(cmd, env, &pid);
			waitpid(pid.pid[0], NULL, 0);
		}
	}
	free(pid.pid);
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
	rl_clear_history();
	return (0);
}


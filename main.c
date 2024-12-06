/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:18:53 by etaquet           #+#    #+#             */
/*   Updated: 2024/12/06 23:46:26 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_graffiti(void)
{
	printf("\x1B[35m    __                    __         __   \x1B[33m___  __");
	printf("______ \x1B[36m __  __   __\n\x1B[35m   / /   ____  ____ _____/ /");
	printf("__  ____/ /  \x1B[33m|__ \\<  / ___/\x1B[36m / / / /  / /\n");
	printf("\x1B[35m  / /   / __ \\/ __ `/ __  / _ \\/ __  /   \x1B[33m__/ //");
	printf(" /\\__ \\\x1B[36m / /_/ /  / / \n\x1B[35m / /___/ /_/ / /_/ / /_/");
	printf(" /  __/ /_/ /   \x1B[33m/ __// /___/ /\x1B[36m/ __  /  /_/  \n");
	printf("\x1B[35m/_____/\\____/\\__,_/\\__,_/\\___/\\__,_/   \x1B[33m");
	printf("/____/_//____/\x1B[36m/_/ /_/  (_)   \n\n");
}

int	check_if_only_space(char *str)
{
	int	i;
	int	v;

	i = 0;
	v = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			i++;
		else
		{
			v++;
			i++;
		}
	}
	return (v);
}

void	free_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		if (args[i])
			free(args[i]);
		i++;
	}
	if (args)
		free(args);
}

char	**getpath(void)
{
	char	**envpath;

	envpath = NULL;
	envpath = ft_split(getenv("PATH"), ':');
	if (envpath)
		return (envpath);
	return (NULL);
}

char    *get_cpath(char *args, char *envpath)
{
	char	*cpath;

	cpath = malloc(sizeof(char) * (ft_strlen(envpath) + ft_strlen(args) + 2));
	if (!cpath)
		return (NULL);
	ft_strcpy(cpath, envpath);
	ft_strcat(cpath, "/");
	ft_strcat(cpath, args);
	return (cpath);
}

char	*get_cmd_path(char *arg)
{
	char	*cpath;
	char	**mp;
	int		i;

	i = 0;
	if (access(arg, X_OK) != -1)
		return (arg);
	mp = getpath();
	if (!mp)
		return (NULL);
	while (mp[i])
	{
		cpath = get_cpath(arg, mp[i]);
		if (access(cpath, X_OK) != -1)
			return (free_args(mp), cpath);
		free(cpath);
		i++;
	}
	free_args(mp);
	return (NULL);
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

void	child_process(char **cmd, char **envp, t_pidstruct	*pid)
{
	char	*actual_cmd;

	actual_cmd = get_cmd_path(cmd[0]);
	pid->pid[0] = fork();
	if (pid->pid[0] == -1)
		perror("Error: Fork failed.\n");
	if (pid->pid[0] == 0)
	{
		if (access(actual_cmd, X_OK) != -1)
			execve(actual_cmd, cmd, envp);
		else
			perror("21sh");
	}
	free(actual_cmd);
}

void    execute_input(char *cwd, char **env, t_pidstruct *pid, char *input)
{
	char	**cmd;
	char    *cmd_path;

	cmd = ft_split(input, ' ');
	if (ft_cd(cwd, cmd))
		return (free_args(cmd));
	cmd_path = get_cmd_path(cmd[0]);
	if (access(cmd_path, X_OK) != -1)
	{
		child_process(cmd, env, pid);
		waitpid(pid->pid[0], NULL, 0);
		free(cmd_path);
	}
	else
		perror("21sh");
	free_args(cmd);
}

int	read_lines(char *cwd, char **env, t_pidstruct *pid)
{
	char	*input;
	char	*tmp_path;
	char	*r_path;

	pid->pid = malloc(sizeof(pid_t));
	signal(SIGQUIT, sigquit_handler);
	tmp_path = get_relative_path(cwd);
	printf("\e[1m\x1B[31m");
	r_path = ft_strjoin(tmp_path, ": \e[m");
	free(tmp_path);
	input = readline(r_path);
	if (input == 0 || !strncmp(input, "exit", ft_strlen(input)))
		return (printf("Exiting 21sh...\n"), free(input),
			free(r_path), free(pid->pid), 1);
	if (input)
	{
		add_history(input);
		execute_input(cwd, env, pid, input);
		free(input);
	}
	free(pid->pid);
	free(r_path);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char		cwd[PATH_MAX];
	t_pidstruct	pid;
	(void)argc;
	(void)argv;

	signal(SIGINT, sigint_handler);
	print_graffiti();
	while (1)
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			perror("getcwd() error");
			exit(1);
		}
		if (read_lines(cwd, env, &pid))
			break ;
	}
	rl_clear_history();
	return (0);
}


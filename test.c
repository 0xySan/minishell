/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:18:53 by etaquet           #+#    #+#             */
/*   Updated: 2024/12/06 22:44:09 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/// to use it : gcc -o test test.c -lreadline -lhistory libft/libft.a

#include "test.h"

void	print_graffiti()
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

char	**getpath()
{
	char	**envpath;
	int		i;

	i = -1;
	envpath = NULL;
	envpath = ft_split(getenv("PATH"), ':');
	if (envpath)
		return (envpath);
	return (NULL);
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
		cpath = malloc(sizeof(char) * (ft_strlen(mp[i]) + ft_strlen(arg) + 2));
		if (!cpath)
			return (NULL);
		ft_strcpy(cpath, mp[i]);
		ft_strcat(cpath, "/");
		ft_strcat(cpath, arg);
		if (access(cpath, X_OK) != -1)
			return (free_args(mp), cpath);
		free(cpath);
		i++;
	}
	free_args(mp);
	return (NULL);
}

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
			if (!ft_strncmp(cmd[1], "~/", 2) && (ft_strlen(cmd[1]) == 2))
				chdir(home);
			else if (check_if_raccessible(cwd, cmd[1]) == -1)
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
}

void    execute_input(char *cwd, char **env, t_pidstruct *pid, char *input)
{
	char	**cmd;
	char    *cmd_path;

	cmd = ft_split(input, ' ');
	if (ft_cd(cwd, input, cmd))
	    return ;
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
	char	*r_path;

	pid->pid = malloc(sizeof(pid_t) * 1);
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
		execute_input(cwd, env, pid, input);
		free(input);
	}
	free(pid->pid);
	free(r_path);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_path		path;
	char		cwd[PATH_MAX];
	t_pidstruct	pid;

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


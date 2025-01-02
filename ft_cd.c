/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:45:59 by etaquet           #+#    #+#             */
/*   Updated: 2025/01/02 16:56:08 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_if_accessible(char *path, char **env)
{
	char		*nw_path;
	struct stat	statbuf;
	char		*cwd;

	stat(path, &statbuf);
	if (access(path, F_OK) != -1 && S_ISDIR(statbuf.st_mode))
	{
		chdir(path);
		return ;
	}
	cwd = ft_getenv(env, "PWD");
	nw_path = malloc(ft_strlen(cwd) + ft_strlen(path) + 2);
	copy_then_cat(nw_path, cwd, "/");
	ft_strcat(nw_path, path);
	stat(nw_path, &statbuf);
	if (access(nw_path, F_OK) != -1 && S_ISDIR(statbuf.st_mode))
		return (chdir(nw_path), free(nw_path));
	if (access(nw_path, F_OK) != -1 || access(path, F_OK) != -1)
		printf("cd: not a directory: %s\n", path);
	else
		printf("cd: no such file or directory: %s\n", path);
	free(nw_path);
}

void	check_if_raccessible(char *path, char **env)
{
	char		*nw_path;
	char		*home;
	char		*tmp;
	struct stat	statbuf;

	home = ft_getenv(env, "HOME");
	if (!home)
		home = getenv("HOME");
	nw_path = malloc(ft_strlen(home) + ft_strlen(path));
	tmp = ft_substr(path, 1, ft_strlen(path));
	copy_then_cat(nw_path, home, tmp);
	stat(nw_path, &statbuf);
	if (access(nw_path, F_OK) != -1 && S_ISDIR(statbuf.st_mode))
		chdir(nw_path);
	else if (access(nw_path, F_OK) != -1)
		printf("cd: not a directory: %s\n", path);
	else
		printf("cd: no such file or directory: %s\n", path);
	free(nw_path);
	if (tmp)
		free(tmp);
}

char	*ft_get_current_dir(void)
{
	char	*cwd;

	cwd = malloc(1024);
	if (!cwd)
		return (NULL);
	if (getcwd(cwd, 1024) == NULL)
	{
		free(cwd);
		return (NULL);
	}
	return (cwd);
}

void	ft_change_dir(char **cmd, char **env)
{
	char	*oldpwd;
	char	*home;

	home = ft_getenv(env, "HOME");
	if (!home)
		home = getenv("HOME");
	oldpwd = ft_getenv(env, "OLDPWD");
	if (!oldpwd)
		oldpwd = ft_getenv(env, "PWD");
	if (!cmd[1] || (cmd[1][0] == '~' && ft_strlen(cmd[1]) == 1))
		chdir(home);
	else if (cmd[1][0] == '-' && ft_strlen(cmd[1]) == 1)
		chdir(oldpwd);
	else if (!cmd[1] || (cmd[1][0] == '~' && ft_strlen(cmd[1]) > 1))
		check_if_raccessible(cmd[1], env);
	else
		check_if_accessible(cmd[1], env);
}

int	ft_cd(char **cmd, char **env)
{
	char	*current_pwd;

	if (ft_strncmp(cmd[0], "cd", 3))
		return (0);
	if (count_args(cmd) > 2)
		return (printf("cd: too many arguments\n"), 1);
	current_pwd = ft_getenv(env, "PWD");
	ft_change_dir(cmd, env);
	ft_change_env(env, "OLDPWD", current_pwd);
	current_pwd = ft_get_current_dir();
	if (current_pwd)
	{
		ft_change_env(env, "PWD", current_pwd);
		free(current_pwd);
	}
	return (1);
}

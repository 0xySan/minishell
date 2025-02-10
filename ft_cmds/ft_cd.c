/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:45:59 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/10 06:59:43 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_if_accessible(char *path, char **env)
{
	struct stat	statbuf;

	stat(path, &statbuf);
	if (access(path, F_OK) != -1 && S_ISDIR(statbuf.st_mode))
		chdir(path);
	else
		perror("cd");
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
		home = ft_getenv(env, "PWD");
	oldpwd = ft_getenv(env, "OLDPWD");
	if (!oldpwd)
		oldpwd = ft_getenv(env, "PWD");
	else if (cmd[1][0] == '-' && ft_strlen(cmd[1]) == 1)
	{
		chdir(oldpwd);
		printf("%s\n", oldpwd);
	}
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

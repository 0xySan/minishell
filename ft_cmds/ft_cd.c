/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:45:59 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/16 06:36:04 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Change to the directory if the path is accessible.
 * @param path Directory path to check.
 */
void	check_if_accessible(char *path)
{
	struct stat	statbuf;

	stat(path, &statbuf);
	if (access(path, F_OK) != -1 && S_ISDIR(statbuf.st_mode))
	{
		chdir(path);
		g_exit_status = 0;
		return ;
	}
	if (access(path, F_OK) == -1)
		dprintf(2, "cd: no such file or directory: %s\n", path);
	else
		dprintf(2, "cd: not a directory: %s\n", path);
	g_exit_status = 1 << 8;
}

/**
 * @brief Retrieves the current working directory.
 * @return Current working directory or NULL on error.
 */
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

/**
 * @brief Changes the current working directory.
 * @param cmd The array of command arguments.
 * @param env The environment variables.
 */
void	ft_change_dir(char **cmd, char **env)
{
	char	*oldpwd;
	char	*home;

	home = ft_getenv(env, "HOME");
	oldpwd = ft_getenv(env, "OLDPWD");
	if (!cmd[1] || !ft_strncmp(cmd[1], "--", 3))
	{
		if (!home)
		{
			ft_dprintf(2, "21sh: cd: HOME not set\n");
			g_exit_status = 1 << 8;
			return ;
		}
		check_if_accessible(home);
		g_exit_status = 0;
	}
	else if (cmd[1][0] == '-' && ft_strlen(cmd[1]) == 1)
	{
		chdir(oldpwd);
		printf("%s\n", oldpwd);
		g_exit_status = 0;
	}
	else
		check_if_accessible(cmd[1]);
}

/**ft_
 * @brief Handles the cd built-in command.
 * @param cmd The array of arguments of the command.
 * @param env The environment.
 * @return 1 if the command is recognized and executed, 0 otherwise.
 * Changes the current directory to the given path.
 * If no path is given, changes to $HOME. If '-', changes to
 * $OLDPWD and prints it.
 */
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

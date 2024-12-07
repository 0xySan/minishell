/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:45:59 by etaquet           #+#    #+#             */
/*   Updated: 2024/12/07 13:37:29 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_then_cat(char *dest, char *fstr, char *sstr)
{
	ft_strcpy(dest, fstr);
	ft_strcat(dest, sstr);
}

void	check_if_accessible(char *cwd, char *path)
{
	char		*nw_path;
	struct stat	statbuf;

	stat(path, &statbuf);
	if (access(path, F_OK) != -1 && S_ISDIR(statbuf.st_mode))
	{
		chdir(path);
		return ;
	}
	nw_path = malloc(ft_strlen(cwd) + ft_strlen(path) + 2);
	copy_then_cat(nw_path, cwd, "/");
	ft_strcat(nw_path, path);
	stat(nw_path, &statbuf);
	if (access(nw_path, F_OK) != -1 && S_ISDIR(statbuf.st_mode))
	{
		chdir(nw_path);
		free(nw_path);
		return ;
	}
	if (access(nw_path, F_OK) != -1 || access(path, F_OK) != -1)
		printf("cd: not a directory: %s\n", path);
	else
		printf("cd: no such file or directory: %s\n", path);
	free(nw_path);
}

void	check_if_raccessible(char *path)
{
	char	*nw_path;
	char	*home;
	char	*tmp;
	struct stat	statbuf;

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

int	ft_cd(char *cwd, char **cmd)
{
	char	*home;

	if (ft_strncmp(cmd[0], "cd", 3))
		return (0);
	home = getenv("HOME");
	if (!cmd[1] || (cmd[1][0] == '~' && ft_strlen(cmd[1]) == 1))
		chdir(home);
	else if (cmd[1][0] == '~' && ft_strlen(cmd[1]) > 1)
	{
		if (!ft_strncmp(cmd[1], "~/", 2) && (ft_strlen(cmd[1]) == 2))
			return (chdir(home), 1);
		check_if_raccessible(cmd[1]);
	}
	else
		check_if_accessible(cwd, cmd[1]);
	return (1);
}

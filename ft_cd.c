/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:45:59 by etaquet           #+#    #+#             */
/*   Updated: 2024/12/07 11:40:36 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	check_if_raccessible(char *path)
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
			chdir(home);
		else if (check_if_raccessible(cmd[1]) == -1)
			perror("cd");
	}
	else
		if (check_if_accessible(cwd, cmd[1]) == -1)
			perror("cd");
	return (1);
}

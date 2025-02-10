/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:52:06 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/10 20:18:05 by hdelacou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_only_space(char *str)
{
	return (*str != '\0' && strchr(str, ' ') == NULL);
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

char	*get_cpath(char *args, char *envpath)
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

char	*get_cmd_path(char *arg, char *path)
{
	char	*cpath;
	char	**mp;
	int		i;

	i = 0;
	if (access(arg, X_OK) != -1 && count_chars(arg, '/') >= 1)
		return (ft_strdup(arg));
	mp = ft_split(path, ':');
	if (!mp)
		return (NULL);
	while (mp[i])
	{
		cpath = get_cpath(arg, mp[i]);
		if (access(cpath, X_OK) != -1 && !ft_strstr(cpath, "//"))
			return (free_args(mp), cpath);
		free(cpath);
		i++;
	}
	free_args(mp);
	return (NULL);
}

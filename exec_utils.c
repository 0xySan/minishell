/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oxy <oxy@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:52:06 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/09 22:34:29 by oxy              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

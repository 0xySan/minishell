/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:52:06 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/23 01:17:20 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Check if a string contains only space characters.
 * @param str The string to check.
 * @retval 1 The string contains at least one non-space character.
 * @retval 0 The string contains only space characters.
 */
int	check_if_only_space(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}

/**
 * Free an array of strings, along with the strings themselves.
 * @param args The array of strings to free.
 */
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

/**
 * Concatenate env path with command path.
 * @param args The command path to concatenate.
 * @param envpath The environment path to concatenate.
 * @return A newly allocated string containing the concatenated paths.
 */
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

/**
 * @brief Get full path of a command by searching in PATH.
 * @param arg Command path to search.
 * @param path PATH env variable.
 * @return Full path of command or NULL if not found.
 */
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:46:49 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/24 22:46:28 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Concatenates three strings.
 * @param s1 First string.
 * @param s2 Second string.
 * @param s3 Third string.
 * @return The concatenated string or NULL on failure.
 */
char	*ft_strjoin_join(const char *s1, const char *s2, const char *s3)
{
	char	*result;
	int		len;

	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	result = malloc(sizeof(char) * (len + 1));
	ft_strcpy((char *)result, (char *)s1);
	ft_strcat((char *)result, (char *)s2);
	ft_strcat((char *)result, (char *)s3);
	return (result);
}

void	edit_lvl(char **env)
{
	char	*lvl;
	int		lvl_int;
	char	*itoa_value;

	lvl = ft_getenv(env, "SHLVL");
	if (!lvl)
		return ;
	lvl_int = ft_atoi(lvl);
	lvl_int++;
	itoa_value = ft_itoa(lvl_int);
	ft_change_env(env, "SHLVL", itoa_value);
	free(itoa_value);
}

void	print_ifpath_ornot(char *path)
{
	struct stat	statbuf;

	stat(path, &statbuf);
	if (access(path, F_OK) != -1 && S_ISDIR(statbuf.st_mode))
	{
		ft_dprintf(2, "21sh: %s: Is a directory\n", path);
		return ;
	}
	if (count_chars(path, '/') > 0)
		ft_dprintf(2, "21sh: %s: No such file or directory\n", path);
	else
		ft_dprintf(2, "21sh: command not found: '%s'\n", path);
}

int	ft_count_first_commands(char **tokens, int num_tokens)
{
	int	i;
	int	count;

	i = 0;
	count = 1;
	while (i < num_tokens)
	{
		if (ft_strcmp(tokens[i], "|") == 0 || ft_strcmp(tokens[i], ">") == 0
			|| ft_strcmp(tokens[i], ">>") == 0)
			count++;
		i++;
	}
	return (count);
}

int	cmds_null_case(t_cmd *cmds, int safe_stdin)
{
	if (cmds == NULL)
	{
		dup2(safe_stdin, STDIN_FILENO);
		close(safe_stdin);
		return (1);
	}
	return (0);
}

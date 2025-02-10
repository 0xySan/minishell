/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 15:51:08 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/11 00:06:17 by hdelacou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Counts the number of command arguments in the given array.
 * @param args Array of string arguments.
 * @return The number of command arguments in the array.
 */
int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

/**
 * @brief Counts the occurrences of a specific character in a string.
 * @param str The string in which to count occurrences of the character.
 * @param chars The character to count within the string.
 * @return The number of times the character appears in the string.
 */
int	count_chars(char *str, char chars)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == chars)
			j++;
		i++;
	}
	return (j);
}

/**
 * @brief Copies then appends strings.
 * @param dest The destination string which will be modified.
 * @param fstr The string to copy into the destination.
 * @param sstr The string to append to the destination.
 */
void	copy_then_cat(char *dest, char *fstr, char *sstr)
{
	ft_strcpy(dest, fstr);
	ft_strcat(dest, sstr);
}

/**
 * Duplicate src and concat sec_src to it.
 * @param src Source string to duplicate.
 * @param sec_src String to append.
 * @return New string or NULL if error.
 */
char	*dup_then_cat(char *src, char *sec_src)
{
	char	*dest;
	int		len;
	int		start;

	if (!src)
		return (0);
	dest = malloc(sizeof(char) * (ft_strlen(src) + ft_strlen(sec_src) + 1));
	if (!dest)
		return (0);
	start = -1;
	while (src[++start])
		dest[start] = src[start];
	dest[start] = '\0';
	ft_strcat(dest, sec_src);
	return (dest);
}

/**
 * @brief Wait for all children of a pipeline context to finish.
 * @param ctx The pipeline context.
 */
void	wait_for_children(t_pipeline_ctx *ctx)
{
	int	i;
	int	status;

	i = 0;
	while (i < ctx->count)
		waitpid(ctx->cmds[i++].pid, &status, 0);
}

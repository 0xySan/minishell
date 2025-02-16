/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 15:51:08 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/16 06:39:51 by etaquet          ###   ########.fr       */
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

int	ft_stralnum(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isalnum(str[i]) || str[i] == '_')
			i++;
		else
			return (0);
	}
	return (1);
}

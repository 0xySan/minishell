/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 22:40:29 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/10 23:52:05 by hdelacou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Allocate a new string and copy len characters from start to it.
 * @param start The string to copy from.
 * @param len The number of characters to copy.
 * @return A newly allocated string, or NULL if memory allocation fails.
 */
char	*alloc_token(const char *start, int len)
{
	char	*token;

	token = malloc(sizeof(char) * (len + 1));
	if (!token)
		return (NULL);
	strncpy(token, start, len);
	token[len] = '\0';
	return (token);
}

/**
 * @brief Add a token to a dynamic array of char*, reallocate if necessary.
 * @param tokens The dynamic array of char*.
 * @param token The token to add to the array.
 * @param count The current count of elements in the array.
 * @param capacity The current capacity of the array.
 * @return 1 if successful, 0 if memory allocation fails.
 */
int	add_token(char ***tokens, char *token, int *count, int *capacity)
{
	char	**tmp;

	(*tokens)[*count] = token;
	(*count)++;
	if (*count >= *capacity)
	{
		*capacity *= 2;
		tmp = realloc(*tokens, sizeof(char *) * (*capacity));
		if (!tmp)
			return (0);
		*tokens = tmp;
	}
	return (1);
}

/**
 * Frees an array of tokens and the tokens themselves.
 * @param tokens The array of tokens to free.
 * @param count The number of tokens in the array.
 */
void	free_tokens(char **tokens, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

/**
 * Extracts a token from the input string based on delimiters.
 * Advances the pointer to the end of the extracted token.
 *
 * @param p Pointer to the input string pointer, which will be updated.
 * @param delimSet Set of delimiter characters used to split the tokens.
 * @return Newly allocated token string or NULL if allocation fails.
 */
char	*get_token(const char **p, const char *delimSet)
{
	const char	*start;
	int			len;
	bool		is_delim;
	char		*token;

	is_delim = (strchr(delimSet, **p) != NULL);
	start = *p;
	while (**p && ((strchr(delimSet, **p) != NULL) == is_delim))
		(*p)++;
	len = *p - start;
	token = alloc_token(start, len);
	return (token);
}

/**
 * @brief Reallocates an array of char* and adds new elements.
 * @param res The array to reallocate.
 * @param info A structure containing information about the array.
 * @param split The new elements to add to the array.
 * @param split_size The number of elements in split.
 * @return 1 if successful, 0 if memory allocation fails.
 */
int	realloc_and_add_tokens(char ***res, t_token_info *info,
	char **split, int split_size)
{
	char	**tmp;
	int		i;

	if (info->total + split_size >= info->capacity)
	{
		info->capacity = (info->total + split_size) * 2;
		tmp = realloc(*res, sizeof(char *) * (info->capacity));
		if (!tmp)
			return (0);
		*res = tmp;
	}
	i = 0;
	while (i < split_size)
	{
		(*res)[info->total] = split[i];
		info->total++;
		i++;
	}
	return (1);
}

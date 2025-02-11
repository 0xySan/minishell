/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_n_keep.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 16:44:34 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/11 18:36:22 by hdelacou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Split a string into tokens based on a delimiter set
 * @param str the string to split
 * @param delimSet the delimiter set
 * @param out_size the output size of the array
 * @return a dynamically allocated array of char* containing the tokens
 */
char	**split_string(const char *str, const char *delimSet, int *out_size)
{
	int				capacity;
	int				count;
	char			**tokens;
	const char		*p;
	char			*token;

	capacity = 10;
	count = 0;
	tokens = malloc(sizeof(char *) * capacity);
	if (!tokens)
		return (NULL);
	p = str;
	while (*p)
	{
		token = get_token(&p, delimSet);
		if (!token)
			return (free_tokens(tokens, count), NULL);
		if (!add_token(&tokens, token, &count, &capacity))
			return (free(token), free_tokens(tokens, count), NULL);
	}
	tokens[count] = NULL;
	*out_size = count;
	return (tokens);
}

/**
 * @brief Split strings in an array by delimiters and merge results.
 * @param arr The array of strings to split.
 * @param delimSet The set of delimiter characters to split each string on.
 * @return Array of char* containing all the resulting tokens.
 */
char	**split_array(char **arr, const char *delimSet)
{
	int				split_size;
	char			**result;
	char			**split_result;
	t_token_info	info;

	info.total = 0;
	info.capacity = 10;
	result = malloc(sizeof(char *) * info.capacity);
	if (!result)
		return (NULL);
	while (*arr)
	{
		split_result = split_string(*arr, delimSet, &split_size);
		if (!split_result)
			return (free_tokens(result, info.total), NULL);
		if (!realloc_and_add_tokens(&result, &info, split_result, split_size))
			return (free(split_result), free_tokens(result, info.total), NULL);
		free(split_result);
		arr++;
	}
	result[info.total] = NULL;
	return (result);
}

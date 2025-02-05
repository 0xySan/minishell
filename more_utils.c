/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 16:44:34 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/05 00:50:02 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*alloc_token(const char *start, int len)
{
	char	*token;

	token = malloc(sizeof(char) * (len + 1));
	if (!token)
		return (NULL);
	strncpy(token, start, len);
	token[len] = '\0';
	return (token);
}

static int	add_token(char ***tokens, char *token, int *count, int *capacity)
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

static void	free_tokens(char **tokens, int count)
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

static char	*get_token(const char **p, const char *delimSet)
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

static int	realloc_and_add_tokens(char ***res, t_token_info *info,
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

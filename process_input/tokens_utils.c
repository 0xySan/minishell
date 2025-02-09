/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oxy <oxy@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 22:40:29 by oxy               #+#    #+#             */
/*   Updated: 2025/02/09 22:42:52 by oxy              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

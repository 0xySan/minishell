/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 16:44:34 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/04 17:21:47 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (malloc(new_size));
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (old_size < new_size)
		ft_memcpy(new_ptr, ptr, old_size);
	else
		ft_memcpy(new_ptr, ptr, new_size);
	free(ptr);
	return (new_ptr);
}

char	**split_string(const char *str, const char *delim, int *count)
{
	const char	*start;
	const char	*end;
	int			capacity;
	char		**result;
	int			len;

	capacity = 10;
	result = malloc(capacity * sizeof(char *));
	*count = 0;
	start = str;
	while (*start)
	{
		end = start;
		while (*end && !ft_strchr(delim, *end))
			end++;
		if (start != end)
		{
			if (*count >= capacity)
			{
				capacity *= 2;
				result = ft_realloc(result, capacity, sizeof(char *));
			}
			len = end - start;
			result[*count] = malloc(len + 1);
			ft_strncpy(result[*count], start, len);
			result[*count][len] = '\0';
			(*count)++;
		}
		if (*end)
		{
			if (*count >= capacity)
			{
				capacity *= 2;
				result = ft_realloc(result, capacity, sizeof(char *));
			}
			result[*count] = malloc(2);
			result[*count][0] = *end;
			result[*count][1] = '\0';
			(*count)++;
			end++;
		}
		start = end;
	}
	return (result);
}

char	**resize_result(char **result, int *capacity)
{
	*capacity *= 2;
	return (ft_realloc(result, *capacity, sizeof(char *)));
}

void	process_input_string(char *input_str, const char *delim,
		t_split_context *context)
{
	int		count;
	char	**split;
	int		j;

	count = 0;
	split = split_string(input_str, delim, &count);
	j = 0;
	while (j < count)
	{
		if (*context->total_count >= *context->capacity)
			context->result = resize_result(context->result, context->capacity);
		context->result[*context->total_count] = split[j];
		(*context->total_count)++;
		j++;
	}
	free(split);
}

char	**split_array(char **input, const char *delim, int *total_count)
{
	int				capacity;
	char			**result;
	int				i;
	t_split_context	context;

	capacity = 10;
	result = malloc(capacity * sizeof(char *));
	if (!result)
		return (NULL);
	*total_count = 0;
	i = 0;
	context.result = result;
	context.total_count = total_count;
	context.capacity = &capacity;
	while (input[i] != NULL)
	{
		process_input_string(input[i], delim, &context);
		i++;
	}
	return (result);
}

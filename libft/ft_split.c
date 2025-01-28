/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 21:33:33 by oxysan            #+#    #+#             */
/*   Updated: 2025/01/28 16:16:57 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(const char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c)
		{
			if (!in_word)
			{
				in_word = 1;
				count++;
			}
		}
		else
			in_word = 0;
		s++;
	}
	return (count);
}

static char	*extract_word(const char *s, int *index, char c)
{
	int		start;
	int		len;
	char	*word;

	start = *index;
	while (s[*index] && s[*index] != c)
		(*index)++;
	len = *index - start;
	word = malloc(len + 1);
	if (word)
	{
		ft_strncpy(word, s + start, len);
		word[len] = '\0';
	}
	return (word);
}

static void	split_words(char const *s, char c, char **result)
{
	int	index;
	int	word_index;

	index = 0;
	word_index = 0;
	while (s[index])
	{
		if (s[index] != c)
			result[word_index++] = extract_word(s, &index, c);
		else
			index++;
	}
	result[word_index] = NULL;
}

char	**ft_split(char const *s, char c)
{
	int		word_count;
	char	**result;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	result = malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	split_words(s, c, result);
	return (result);
}

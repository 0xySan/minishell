/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:44:43 by etaquet           #+#    #+#             */
/*   Updated: 2025/01/08 16:46:48 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	copy_array(char **dest, char **src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i - 2] = NULL;
}

void	swap(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	sort_strings(char **array)
{
	int	i;
	int	min_index;
	int	j;

	i = 0;
	while (array[i] != NULL)
	{
		min_index = i;
		j = i + 1;
		while (array[j] != NULL)
		{
			if (ft_strcmp(array[j], array[min_index]) < 0)
				min_index = j;
			j++;
		}
		if (min_index != i)
			swap(&array[i], &array[min_index]);
		i++;
	}
}

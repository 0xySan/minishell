/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:44:43 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/10 23:44:30 by hdelacou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Copy src array to dest array.
 * @param dest Array to copy to
 * @param src Array to copy from
 */
void	copy_array(char **dest, char **src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = NULL;
}

/**
 * Swap the value of two pointers.
 * @param a Pointer to swap
 * @param b Pointer to swap
 */
void	swap(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

/**
 * Sorts an array of strings in ascending order
 * using selection sort algorithm.
 * @param array Array of strings to sort.
 */
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

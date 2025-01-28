/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:56:24 by etaquet           #+#    #+#             */
/*   Updated: 2025/01/28 16:13:15 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_only_char(char *str, int start, char car)
{
	int	i;

	i = start;
	while (str[i] == car)
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

void	ft_echo(char **cmd)
{
	int	i;
	int	n;

	i = 1;
	n = 0;
	if ((cmd[i][0] == '-') && check_if_only_char(cmd[i], 1, 'n'))
	{
		i++;
		n++;
	}
	while (cmd[i] && cmd[i + 1])
	{
		printf("%s ", cmd[i]);
		i++;
	}
	if (cmd[i])
		printf("%s", cmd[i]);
	if (!n)
		printf("\n");
}

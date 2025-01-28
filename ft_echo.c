/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:56:24 by etaquet           #+#    #+#             */
/*   Updated: 2025/01/28 20:23:52 by hdelacou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_only_char(char *str, int start, char car)
{
	int	i;

	if (!str)
		return (0);
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
	n = (cmd[i] && cmd[i][0] == '-' && check_if_only_char(cmd[i], 1, 'n'));
	if (n)
		i++;
	while (cmd[i] && cmd[i + 1])
		printf("%s ", cmd[i++]);
	if (cmd[i])
		printf("%s", cmd[i]);
	if (!n)
		printf("\n");
}

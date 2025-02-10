/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:56:24 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/10 23:25:31 by hdelacou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

	i = 1;
	if (cmd[i] && cmd[i][0] == '-' && check_if_only_char(cmd[i], 1, 'n'))
		i++;
	while (cmd[i] && cmd[i + 1])
		printf("%s ", cmd[i++]);
	if (cmd[i])
		printf("%s", cmd[i]);
	if (cmd[i] && cmd[i][0] != '-' && !check_if_only_char(cmd[i], 0, 'n'))
		printf("\n");
}

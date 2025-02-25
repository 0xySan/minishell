/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:56:24 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/25 15:46:51 by etaquet          ###   ########.fr       */
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

void	ft_echo(char **cmd, int *exit_code)
{
	int	i;
	int	v;

	i = 1;
	v = 0;
	while (cmd[i] && cmd[i][0] == '-' && check_if_only_char(cmd[i], 1, 'n'))
	{
		i++;
		v++;
	}
	while (cmd[i] && cmd[i + 1])
		printf("%s ", cmd[i++]);
	if (cmd[i])
		printf("%s", cmd[i]);
	if (v == 0)
		printf("\n");
	*exit_code = 0;
}

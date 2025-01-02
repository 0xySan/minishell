/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:56:24 by etaquet           #+#    #+#             */
/*   Updated: 2025/01/02 17:03:21 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(char **cmd)
{
	int	i;
	int	n;

	n = 0;
	if (!ft_strncmp(cmd[1], "-n", 2))
	{
		i = 2;
		n = 1;
	}
	else
		i = 1;
	while (cmd[i + 1])
	{
		printf("%s ", cmd[i]);
		i++;
	}
	if (!n)
		printf("%s\n", cmd[i]);
	else
		printf("%s", cmd[i]);
}

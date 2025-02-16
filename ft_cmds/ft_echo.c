/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:56:24 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/16 06:27:22 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Check if string contains only a character from a given start index.
 *
 * @param str The string to check.
 * @param start The starting index of the string to check from.
 * @param car The character to check for.
 *
 * @retval 1 String contains only the character from start.
 * @retval 0 Otherwise.
 */
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

/**
 * ft_echo: Execute the echo command.
 *
 * Prints arguments separated by spaces. If "-n" is the first argument,
 * does not print a newline at the end.
 *
 * @param cmd: A 2D array of strings, where the first element is the command name
 * "echo", and the rest of the elements are the command arguments.
 */
void	ft_echo(char **cmd)
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
	g_exit_status = 0;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 06:41:59 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/18 08:13:28 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_overflow(const char *str)
{
	char		*endptr;
	long long	num;

	num = ft_strtoll(str, &endptr, 10);
	errno = 0;
	if (endptr == str)
		return (0);
	if (errno == ERANGE || num >= LLONG_MAX || num < LLONG_MIN)
		return (1);
	return (0);
}

int	ft_alnum_exit(char **exit_input, char *input)
{
	int		i;

	i = 0;
	while (exit_input[1][i])
	{
		if (ft_isalpha(exit_input[1][i]) || check_overflow(exit_input[1]))
		{
			ft_dprintf(2, "21sh: exit: %s: numeric argument required\n",
				exit_input[1]);
			g_exit_status = 2;
			free(input);
			free_args(exit_input);
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_exit(char *input, char **env)
{
	char	**exit_input;
	int		args;
	char	*itoa_result;

	if (!input)
		return (1);
	add_history(input);
	exit_input = parse_input(input, env);
	args = count_args(exit_input);
	if (!exit_input[1])
		return (free(input), free_args(exit_input), 1);
	if (ft_alnum_exit(exit_input, input))
		return (1);
	if (args > 2)
	{
		ft_dprintf(2, "21sh: exit too many arguments\n");
		g_exit_status = 1 << 8;
		free(input);
		free_args(exit_input);
		return (0);
	}
	g_exit_status = ft_atoi(exit_input[1]);
	free(input);
	free_args(exit_input);
	return (1);
}

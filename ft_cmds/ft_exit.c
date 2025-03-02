/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 06:41:59 by etaquet           #+#    #+#             */
/*   Updated: 2025/03/02 19:13:10 by etaquet          ###   ########.fr       */
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

int	ft_alnum_exit(char **exit_input, char *input, int *exit_code)
{
	int		i;

	i = 0;
	if (exit_input[1][0] == '-' && exit_input[1][1] == '\0')
	{
		ft_dprintf(2, "21sh: exit: %s: numeric argument required\n",
			exit_input[1]);
		*exit_code = 2;
		free(input);
		return (free_args(exit_input), 1);
	}
	while (exit_input[1][i])
	{
		if (ft_isalpha(exit_input[1][i]) || check_overflow(exit_input[1]))
		{
			ft_dprintf(2, "21sh: exit: %s: numeric argument required\n",
				exit_input[1]);
			*exit_code = 2;
			free(input);
			free_args(exit_input);
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_exit(char *input, char **env, int *exit_code)
{
	char	**exit_input;
	int		args;

	if (!input)
		return (*exit_code = WEXITSTATUS(*exit_code), 1);
	add_history(input);
	exit_input = parse_input(input, env, exit_code);
	args = count_args(exit_input);
	if (!exit_input[1])
		return (free(input), free_args(exit_input),
			*exit_code = WEXITSTATUS(*exit_code), 1);
	if (ft_alnum_exit(exit_input, input, exit_code))
		return (1);
	if (args > 2)
	{
		ft_dprintf(2, "21sh: exit too many arguments\n");
		*exit_code = 1 << 8;
		free(input);
		free_args(exit_input);
		return (0);
	}
	*exit_code = ft_atoi(exit_input[1]);
	free(input);
	free_args(exit_input);
	return (1);
}

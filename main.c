/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:18:53 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/25 13:40:45 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_graffiti(void)
{
	printf("\x1B[35m    __                    __         __   \x1B[33m___  __");
	printf("_ \x1B[36m_____ __  __   __\n\x1B[35m   / /   ____  ____ _____/ /");
	printf("__  ____/ /  \x1B[33m|__ \\<  /\x1B[36m/ ___// / / /  / /\n");
	printf("\x1B[35m  / /   / __ \\/ __ `/ __  / _ \\/ __  /   \x1B[33m__/ //");
	printf(" / \x1B[36m\\__ \\/ /_/ /  / / \n\x1B[35m / /___/ /_/ / /_/ / /_/");
	printf(" /  __/ /_/ /   \x1B[33m/ __// / \x1B[36m___/ / __  /  /_/  \n");
	printf("\x1B[35m/_____/\\____/\\__,_/\\__,_/\\___/\\__,_/   \x1B[33m");
	printf("/____/_/ \x1B[36m/____/_/ /_/  (_)   \n\n");
}

char	*get_relative_path(char *pwd, char **env)
{
	char	*home;
	size_t	home_len;
	size_t	pwd_len;
	char	*t_path;
	char	*r_path;

	home = ft_getenv(env, "HOME");
	if (!home)
		return (ft_strjoin_join("\e[1m\x1B[33mMINISHELL \e[1m\x1B[31m", pwd,
				": \e[m"));
	home_len = ft_strlen(home);
	pwd_len = ft_strlen(pwd);
	if (home_len > pwd_len || ft_strncmp(home, pwd, home_len) != 0)
		return (ft_strjoin_join("\e[1m\x1B[33mMINISHELL \e[1m\x1B[31m", pwd,
				": \e[m"));
	t_path = ft_strjoin_join("~", pwd + home_len, ": \e[m");
	r_path = ft_strjoin("\e[1m\x1B[33mMINISHELL \e[1m\x1B[31m", t_path);
	free(t_path);
	return (r_path);
}

void	sigcode(int *exit_code)
{
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		*exit_code = 130 << 8;
	}
	if (g_signal == SIGQUIT)
	{
		g_signal = 0;
		*exit_code = 131 << 8;
	}
}

int	read_lines(char *cwd, char ***env, int *exit_code)
{
	char	*input;
	char	*rev_path;
	t_free	free_value;

	setup_signals(1);
	rev_path = get_relative_path(cwd, *env);
	free_value.relative_path = rev_path;
	free_value.exit_code = exit_code;
	input = readline(rev_path);
	sigcode(exit_code);
	if (input == 0 || (!strncmp(input, "exit", 4)
			&& ft_strlen(input) >= 4))
	{
		free(rev_path);
		return (ft_exit(input, *env, exit_code));
	}
	if (input && check_if_only_space(input))
	{
		add_history(input);
		if (execute_input(env, input, &free_value, exit_code) == 1)
			return (ft_exit(input, *env, exit_code));
		free(input);
	}
	free(rev_path);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char	cwd[PATH_MAX];
	int		exit_code;

	(void)argc;
	(void)argv;
	printf("\033[H\033[J");
	print_graffiti();
	exit_code = 0;
	env = dup_all_env(env);
	edit_lvl(env);
	while (1)
	{
		getcwd(cwd, sizeof(cwd));
		if (read_lines(cwd, &env, &exit_code))
			break ;
	}
	rl_clear_history();
	ft_free_env(env);
	return (exit_code);
}

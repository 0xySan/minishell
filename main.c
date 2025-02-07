/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:18:53 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/08 00:16:11 by etaquet          ###   ########.fr       */
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

	home = ft_getenv(env, "HOME");
	if (!home)
		return (ft_strdup(pwd));
	home_len = ft_strlen(home);
	pwd_len = ft_strlen(pwd);
	if (home_len > pwd_len || ft_strncmp(home, pwd, home_len) != 0)
		return (ft_strdup(pwd));
	return (ft_strjoin("~", pwd + home_len));
}

int	read_lines(char *cwd, char ***env, int *exit_status)
{
	char	*input;
	char	*tmp_path;
	char	*r_path;

	tmp_path = get_relative_path(cwd, *env);
	printf("\e[1m\x1B[33mMINISHELL \e[1m\x1B[31m");
	r_path = ft_strjoin(tmp_path, ": \e[m");
	free(tmp_path);
	input = readline(r_path);
	if (input == 0 || (!strncmp(input, "exit", ft_strlen(input))
			&& ft_strlen(input) == 4))
		return (printf("Exiting 21sh...\n"), free(input), free(r_path), 1);
	if (input && check_if_only_space(input))
	{
		add_history(input);
		execute_input(env, input, exit_status);
		free(input);
	}
	free(r_path);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char	cwd[PATH_MAX];
	int		exit_status;

	(void)argc;
	(void)argv;
	exit_status = 0;
	printf("\033[H\033[J");
	env = dup_all_env(env);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	print_graffiti();
	while (1)
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
			exit(1);
		if (read_lines(cwd, &env, &exit_status))
			break ;
	}
	rl_clear_history();
	ft_free_env(env);
	return (0);
}

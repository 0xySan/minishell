/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:18:53 by etaquet           #+#    #+#             */
/*   Updated: 2024/12/07 12:44:59 by etaquet          ###   ########.fr       */
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

char	*get_relative_path(char *pwd)
{
	char	*home;
	size_t	home_len;
	size_t	pwd_len;

	home = getenv("HOME");
	if (!home)
		return (ft_strdup(pwd));
	home_len = ft_strlen(home);
	pwd_len = ft_strlen(pwd);
	if (home_len > pwd_len || ft_strncmp(home, pwd, home_len) != 0)
		return (ft_strdup(pwd));
	return (ft_strjoin("~", pwd + home_len));
}

int	read_lines(char *cwd, char **env, t_pidstruct *pid)
{
	char	*input;
	char	*tmp_path;
	char	*r_path;

	pid->pid = malloc(sizeof(pid_t));
	signal(SIGQUIT, sigquit_handler);
	tmp_path = get_relative_path(cwd);
	printf("\e[1m\x1B[31m");
	r_path = ft_strjoin(tmp_path, ": \e[m");
	free(tmp_path);
	input = readline(r_path);
	if (input == 0 || (!strncmp(input, "exit", ft_strlen(input))
			&& ft_strlen(input) == 4))
		return (printf("Exiting 21sh...\n"), free(input),
			free(r_path), free(pid->pid), 1);
	if (input && check_if_only_space(input))
	{
		add_history(input);
		execute_input(cwd, env, pid, input);
		free(input);
	}
	free(pid->pid);
	free(r_path);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char		cwd[PATH_MAX];
	t_pidstruct	pid;

	(void)argc;
	(void)argv;
	signal(SIGINT, sigint_handler);
	print_graffiti();
	while (1)
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			perror("getcwd() error");
			exit(1);
		}
		if (read_lines(cwd, env, &pid))
			break ;
	}
	rl_clear_history();
	return (0);
}

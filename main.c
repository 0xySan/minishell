/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:18:53 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/18 08:26:18 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status;

/**
 * Print a stylized ASCII art graffiti to the console.
 */
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

/**
 * Get the relative path by replacing the home directory with '~'.
 * @param pwd The current working directory.
 * @param env The environment variables.
 * @return A newly allocated string with the relative path or original `pwd`.
 */
char	*get_relative_path(char *pwd, char **env)
{
	char	*home;
	size_t	home_len;
	size_t	pwd_len;
	char	*t_path;
	char	*r_path;

	home = ft_getenv(env, "HOME");
	if (!home)
		return (ft_strdup(pwd));
	home_len = ft_strlen(home);
	pwd_len = ft_strlen(pwd);
	if (home_len > pwd_len || ft_strncmp(home, pwd, home_len) != 0)
		return (ft_strdup(pwd));
	t_path = ft_strjoin("~", pwd + home_len);
	r_path = ft_strjoin(t_path, ": \e[m");
	free(t_path);
	t_path = NULL;
	t_path = ft_strjoin("\e[1m\x1B[33mMINISHELL \e[1m\x1B[31m", r_path);
	free(r_path);
	return (t_path);
}

/**
 * Reads user input and executes it.
 * @param cwd The current working directory.
 * @param env The environment variables.
 * @param exit_status The exit status of the last executed command.
 * @return 1 if the user wants to exit the shell, else 0.
 */
int	read_lines(char *cwd, char ***env)
{
	char	*input;
	char	*rev_path;

	rev_path = get_relative_path(cwd, *env);
	input = readline(rev_path);
	if (input == 0 || (!strncmp(input, "exit", 4)
			&& ft_strlen(input) >= 4))
	{
		free(rev_path);
		return (ft_exit(input, *env));
	}
	if (input && check_if_only_space(input))
	{
		add_history(input);
		if (execute_input(env, input, cwd) == 1)
			return (ft_exit(input, *env));
		free(input);
	}
	free(rev_path);
	return (0);
}

/**
 * Main entry point of the program.
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @param env The environment variables.
 * @return 0 on success, 1 if an error occurred.
 * Main entry point. Initializes shell,
 * reads user input in a loop until exit.
 */
int	main(int argc, char **argv, char **env)
{
	char	cwd[PATH_MAX];

	(void)argc;
	(void)argv;
	printf("\033[H\033[J");
	print_graffiti();
	g_exit_status = 0;
	env = dup_all_env(env);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	while (1)
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
			exit(1);
		if (read_lines(cwd, &env))
			break ;
	}
	rl_clear_history();
	ft_free_env(env);
	return (g_exit_status);
}

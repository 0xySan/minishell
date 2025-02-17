/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:18:53 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/17 22:31:23 by hdelacou         ###   ########.fr       */
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

	home = ft_getenv(env, "HOME");
	if (!home)
		return (ft_strdup(pwd));
	home_len = ft_strlen(home);
	pwd_len = ft_strlen(pwd);
	if (home_len > pwd_len || ft_strncmp(home, pwd, home_len) != 0)
		return (ft_strdup(pwd));
	return (ft_strjoin("~", pwd + home_len));
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
	char	*tmp_path;
	char	*t_path;
	char	*r_path;

	tmp_path = get_relative_path(cwd, *env);
	t_path = ft_strjoin(tmp_path, ": \e[m");
	r_path = ft_strjoin("\e[1m\x1B[33mMINISHELL \e[1m\x1B[31m", t_path);
	free(tmp_path);
	free(t_path);
	input = readline(r_path);
	if (input == 0 || (!strncmp(input, "exit", 4)
			&& ft_strlen(input) >= 4))
	{
		free(r_path);
		return (ft_exit(input));
	}
	if (input && check_if_only_space(input))
	{
		add_history(input);
		execute_input(env, input);
		free(input);
	}
	free(r_path);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:28:07 by etaquet           #+#    #+#             */
/*   Updated: 2025/01/08 16:45:49 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <linux/limits.h>
# include "libft/libft.h"
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>

typedef struct s_pidstruct
{
	pid_t	*pid;
}	t_pidstruct;

void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	check_if_accessible(char *path, char **env);
void	check_if_raccessible(char *path, char **env);
int		ft_cd(char **cmd, char **env);
void	execute_input(char ***env, t_pidstruct *pid, char *input);
void	child_process(char **cmd, char *actual_cmd, char **envp,
			t_pidstruct *pid);
char	*get_cmd_path(char *arg, char *path);
char	*get_cpath(char *args, char *envpath);
int		check_if_only_space(char *str);
void	free_args(char **args);
int		count_chars(char *str, char chars);
int		count_args(char **args);
void	copy_then_cat(char *dest, char *fstr, char *sstr);
void	ft_change_env(char **env, char *old_env, char *new_env);
char	**dup_all_env(char **env, int size);
char	*dup_then_cat(char *src, char *sec_src);
char	*ft_getenv(char **env, char *search_env);
void	ft_free_env(char **env);
void	ft_export(char ***env, char *old_env, char *new_env);
void	ft_unset(char **env, char *rev_env);
void	ft_show_env(char **env);
void	ft_echo(char **cmd);
void	sort_strings(char **array);
void	copy_array(char **dest, char **src);

#endif
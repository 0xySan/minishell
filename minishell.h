/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:28:07 by etaquet           #+#    #+#             */
/*   Updated: 2024/12/06 23:53:34 by etaquet          ###   ########.fr       */
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

typedef struct s_pidstruct
{
	pid_t	*pid;
}	t_pidstruct;

void	sigint_handler(int sig);
void	sigquit_handler(int sig);
int		check_if_accessible(char *cwd, char *path);
int		check_if_raccessible(char *path);
int		ft_cd(char *cwd, char **cmd);
void	execute_input(char *cwd, char **env, t_pidstruct *pid, char *input);
void	child_process(char **cmd, char **envp, t_pidstruct	*pid);
char	*get_cmd_path(char *arg);
char	*get_cpath(char *args, char *envpath);
int		check_if_only_space(char *str);
void	free_args(char **args);
char	**getpath(void);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:28:07 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/07 21:41:26 by hdelacou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
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
# include <stdbool.h>

typedef struct s_pidstruct
{
	pid_t	*pid;
}	t_pidstruct;

typedef struct s_cmd
{
	char	**args;
	int		input_fd;
	int		output_fd;
	pid_t	pid;
}	t_cmd;

typedef struct s_token_info
{
	int	total;
	int	capacity;
}	t_token_info;

typedef struct s_pipeline_ctx
{
	t_cmd	*cmds;
	int		count;
	char	**env;
	int		prev_pipe;
}	t_pipeline_ctx;

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
void	ft_parse_pipeline(char **tokens, int num_tokens, char **env);
void	ft_parse_redirection(t_cmd *cmd, char **tokens, int *i);
void	ft_execute(t_cmd *cmd, char **env);
char	**split_array(char **arr, const char *delimSet);
int		ft_count_args(char **tokens, int num_tokens, int start);
int		ft_fill_args(t_cmd *cmd, char **tokens, int num_tokens, int start);
void	setup_pipe(t_pipeline_ctx *ctx, int index, int pipe_fds[2]);
void	cleanup_fds(t_cmd *cmd);
void	execute_command(t_pipeline_ctx *ctx, int index);
t_cmd	*ft_parse_commands(char **tokens, int num_tokens);
int		ft_process_one_command(char **tokens, int num_tokens, int start,
			t_cmd *cmd);
int		ft_count_commands(char **tokens, int num_tokens);

#endif

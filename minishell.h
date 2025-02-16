/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:28:07 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/16 02:50:19 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include "ft_dprintf/ft_printf.h"

typedef struct s_cmd
{
	char		**args;
	int			input_fd;
	int			output_fd;
	pid_t		pid;
}				t_cmd;

typedef struct s_token_info
{
	int			total;
	int			capacity;
}				t_token_info;

typedef struct s_pipeline_ctx
{
	t_cmd		*cmds;
	int			count;
	char		**env;
	int			prev_pipe;
}				t_pipeline_ctx;

typedef struct s_parser
{
	const char	*input;
	size_t		i;
	char		*result;
	size_t		j;
	int			in_single;
	int			in_double;
}				t_parser;

typedef struct s_buffer
{
	char	*str;
	size_t	len;
	size_t	capacity;
	char	**env;
}				t_buffer;

typedef struct s_buf
{
	char	*buf;
	int		len;
	int		cap;
}				t_buf;

typedef struct s_tokens
{
	char	**arr;
	int		count;
	int		cap;
	char	**env;
	int		*exit_status;
}				t_tokens;

typedef struct s_state
{
	const char	*input;
	int			i;
	int			quote;
}				t_state;

// ft_CMDS
// ft_cd
void			check_if_accessible(char *path, char **env);
char			*ft_get_current_dir(void);
void			ft_change_dir(char **cmd, char **env);
int				ft_cd(char **cmd, char **env);
// ft_echo
int				check_if_only_char(char *str, int start, char car);
void			ft_echo(char **cmd);
// ft_ENV
// dup_env
char			**dup_all_env(char **env);
// ft_change_env
void			ft_change_env(char **env, char *old_env, char *new_env);
// ft_env
char			*ft_getenv(char **env, char *search_env);
void			ft_show_env(char **env);
void			ft_free_env(char **env);
// ft_export_n_unset
char			**realloc_tab(char ***env);
void			ft_show_export(char **env);
void			ft_export(char ***env, char *old_env, char *new_env);
void			ft_unset(char **env, char *rev_env);
// f_sort_array
void			copy_array(char **dest, char **src);
void			swap(char **a, char **b);
void			sort_strings(char **array);
// FT_PROCESS_INPUT
// handles
int				handle_double_quote(t_state *s);
int				handle_single_quote(t_state *s);
int				handle_space(t_state *s, t_buf *t, t_tokens *tok);
int				handle_error(t_state *s, t_buf *t, t_tokens *tok);
int				handle_dollar(t_state *s, t_buf *t, t_tokens *tok);
// preprocess_input
void			insert_var_value(char *value, t_buf *t, t_tokens *tok);
void			expand_var(t_state *s, t_buf *t, t_tokens *tok);
void			process_char(t_state *s, t_buf *t, t_tokens *tok);
char			**parse_input(const char *input, char **env, int *exit_status);
// prepocess_utils
void			extract_var_name(t_state *s, char *var_name);
void			append_char(t_buf *t, char c);
void			append_str(t_buf *t, const char *s);
void			plus_token(t_tokens *tok, char *token);
// split_n_keep
char			**split_string(const char *str, const char *delimSet,
					int *out_size);
char			**split_array(char **arr, const char *delimSet);
// tokens_utils
char			*alloc_token(const char *start, int len);
int				add_token(char ***tokens, char *token, int *count,
					int *capacity);
void			free_tokens(char **tokens, int count);
char			*get_token(const char **p, const char *delimSet);
int				realloc_and_add_tokens(char ***res, t_token_info *info,
					char **split, int split_size);
// cmds_handler
void			ft_execute(t_cmd *cmd, char **env);
int				ft_count_commands(char **tokens, int num_tokens);
int				ft_process_one_command(char **tokens, int num_tokens, int start,
					t_cmd *cmd);
t_cmd			*ft_parse_commands(char **tokens, int num_tokens);
void			execute_command(t_pipeline_ctx *ctx, int index);
// exec_input
int				execute_ft_cmds_export(char **cmd, char ***env);
int				execute_ft_cmds(char **cmd, char ***env);
void			execute_input(char ***env, char *input, int *exit_status);
// exec_utils
int				check_if_only_space(char *str);
void			free_args(char **args);
char			*get_cpath(char *args, char *envpath);
char			*get_cmd_path(char *arg, char *path);
// fds_handler
void			ft_parse_redirection(t_cmd *cmd, char **tokens, int *i);
// main
void			print_graffiti(void);
char			*get_relative_path(char *pwd, char **env);
int				read_lines(char *cwd, char ***env, int *exit_status);
int				main(int argc, char **argv, char **env);
// pipes_handler
int				ft_count_args(char **tokens, int num_tokens, int start);
int				ft_fill_args(t_cmd *cmd, char **tokens, int num_tokens,
					int start);
void			setup_pipe(t_pipeline_ctx *ctx, int index, int pipe_fds[2]);
void			cleanup_fds(t_cmd *cmd);
void			ft_parse_pipeline(char **tokens, int num_tokens, char **env,
					int *exit_status);
// sighandler
void			sigint_handler(int sig);
void			sigquit_handler(int sig);
// utils
int				count_args(char **args);
int				count_chars(char *str, char chars);
void			copy_then_cat(char *dest, char *fstr, char *sstr);
char			*dup_then_cat(char *src, char *sec_src);
void			wait_for_children(t_pipeline_ctx *ctx);

#endif

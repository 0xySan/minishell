/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:28:07 by etaquet           #+#    #+#             */
/*   Updated: 2025/03/02 19:10:22 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "ft_dprintf/ft_printf.h"
# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
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

extern volatile sig_atomic_t	g_signal;

typedef struct s_cmd
{
	char		**args;
	int			input_fd;
	int			output_fd;
	int			count;
	char		**env;
	int			*exit_code;
	pid_t		pid;
}				t_cmd;

typedef struct s_pipeline_ctx
{
	t_cmd		*cmds;
	int			count;
	char		**env;
	int			prev_pipe;
}				t_pipeline_ctx;

typedef struct s_buf
{
	char		*buf;
	int			len;
	int			cap;
}				t_buf;

typedef struct s_tokens
{
	char		**arr;
	int			count;
	int			cap;
	char		**env;
}				t_tokens;

typedef struct s_state
{
	const char	*input;
	int			i;
	int			quote;
}				t_state;

typedef struct s_free
{
	char		**parsed_input;
	char		***env;
	char		*relative_path;
	t_cmd		*cmds;
	int			cmd_count;
	int			*exit_code;
}				t_free;

typedef struct s_pid_struct
{
	int		index;
	int		pipe_fds[2];
	int		use_pipe;
	pid_t	pid;
}				t_pid_struct;

//////////////// CMDS_EXEC
// cmds_execute
/**
 * @brief Executes a command with redirection and built-in support.
 * Handles redirection, tries to execute as built-in, then searches in PATH.
 * If execution fails, prints an error message and exits with status 127.
 * @param cmd t_cmd structure with command and arguments.
 * @param free_value Structure containing resources to be freed before exit.
 * @param new_pid Structure containing information about the new process.
 */
void			ft_execute(t_pipeline_ctx *ctx, t_free *free_value,
					t_pid_struct *new_pid);
/**
 * Execute a command in a pipeline context.
 * Sets up pipes, forks, executes the command and handles FDs.
 * @param ctx: Pipeline context.
 * @param index: Index of the command to be executed.
 * @param free_value: Structure containing resources to be freed before exit.
 */
void			execute_command(t_pipeline_ctx *ctx, int index,
					t_free *free_value);
/**
 * @brief
 * 
 * This function checks the input and output file descriptors for the command
 * at the current index. If either of the file descriptors is invalid (-1),
 * it closes any valid file descriptors and frees resources before exiting.
 * 
 * If the input file descriptor is valid and not equal to STDIN_FILENO,
 * it duplicates it to STDIN_FILENO and closes the original file descriptor.
 * 
 * Similarly, if the output file descriptor is valid and not equal
 * to STDOUT_FILENO,
 * it duplicates it to STDOUT_FILENO and closes the original file descriptor.
 * @param ctx: A pointer to the pipeline context structure.
 * @param free_value: A pointer to the structure containing resources
 * to be freed.
 * @param new_pid: A pointer to the structure containing the new process
 * ID and index.
 * 
 */
void			handle_dup2(t_pipeline_ctx *ctx, t_free *free_value,
					t_pid_struct *new_pid);
/**
 * @brief
 * This function handles the behavior of a newly created process in a command
 * pipeline. If the PID is zero, it means the current process is the
 * child process, and it performs necessary file descriptor manipulations
 * and executes the command.
 * If the PID is negative, it indicates a fork error, and the function exits.
 * For positive PIDs, it handles closing the read end of the pipe if necessary.
 * @param ctx: Pointer to the pipeline context structure containing command
 * information.
 * @param free_value: Pointer to a structure used for freeing resources.
 * @param new_pid: Pointer to a structure containing the new PID and related
 * information.
 */
void			handle_pid(t_pipeline_ctx *ctx, t_free *free_value,
					t_pid_struct *new_pid);
/**
 * @brief Close all file descriptors in a pipeline context,
 * except for the one at a given command index.
 * @param ctx Pipeline context.
 * @param index Index of the command whose
 * file descriptors should not be closed.
 */
void			close_fds(t_pipeline_ctx *ctx, int index);
// cmds_handler
/**
 * @brief Frees all allocated memory and exits with a given status code.
 * @param ctx Pipeline context with allocated memory for the commands.
 * @param free_value Structure containing pointers to allocated memory to free.
 * @param exit_code Status code to exit with.
 * @param new_pid Structure containing information about the new process.
 */
void			free_before_exit(t_pipeline_ctx *ctx, t_free *free_value,
					int exit_code, t_pid_struct *new_pid);
/**
 * @brief Parses the given tokens into an array of command structures.
 * Parses tokens into an array of command structures.
 * @param tokens Array of string tokens representing the commands and arguments.
 * @param num_tokens Number of tokens in the tokens array.
 * @param env The environment to use environment variables like the PATH.
 * @param exit_code Status code to exit with.
 * @return Pointer to an array of t_cmd structures representing parsed commands.
 */
t_cmd			*ft_parse_commands(char **tokens, int num_tokens, char **env,
					int *exit_code);
/**
 * @brief Counts the number of commands in the given tokens.
 * Counts commands in tokens by counting "|" tokens.
 * @param tokens Array of string tokens representing the commands and arguments.
 * @param num_tokens Number of tokens in the tokens array.
 * @return The number of commands in the tokens array.
 */
int				ft_count_commands(char **tokens, int num_tokens);
/**
 * @brief Handles the creation of a new t_pid_struct and initializes its values.
 *
 * This function allocates memory for a new t_pid_struct, initializes its
 * members with the provided parameters, and returns a pointer to the newly
 * created structure. If memory allocation fails, the function will terminate
 * the program with an exit status of EXIT_FAILURE.
 *
 * @param index The index to be assigned to the new t_pid_struct.
 * @param pipe_fds An array of two integers representing the file descriptors
 *                 for the pipe.
 * @param use_pipe A flag indicating whether the pipe should be used (non-zero)
 *                 or not (zero).
 * @param pid The process ID to be assigned to the new t_pid_struct.
 * @return A pointer to the newly created t_pid_struct.
 */
t_pid_struct	*handle_new_pid(int index, int *pipe_fds, int use_pipe,
					pid_t pid);
/**
 * @brief Process one command by filling the args field of the given t_cmd.
 * Processes one command by filling the args field of the given t_cmd.
 * @param tokens Array of string tokens representing the commands and arguments.
 * @param num_tokens Number of tokens in the tokens array.
 * @param start Starting index in the tokens array for the command.
 * @param cmd Pointer to a t_cmd structure to be filled.
 * @return The index of the next token after the command, or num_tokens if the
 * command is the last one.
 */
int				ft_process_one_command(char **tokens, int num_tokens, int start,
					t_cmd *cmd);
// exec_input
/**
 * @brief Execute the export command.
 * @param cmd The array of command arguments.
 * @param env The environment variables.
 * @return 1 if the command is recognized and executed, 0 otherwise.
 * Handles the export command. Prints current environment if no args provided;
 * adds variables to environment if args are given.
 */
int				execute_ft_cmds_export(char **cmd, char ***env, int *exit_code);
/**
 * @brief Execute a built-in command.
 * @param cmd The array of command arguments.
 * @param env The environment variables.
 * @param exit_code The exit status of the last executed command.
 * @return 1 if the command is recognized and executed, 0 otherwise.
 */
int				execute_ft_cmds(char **cmd, char ***env, int *exit_code);
/**
 * Execute a single command line.
 * @param env The environment variables.
 * @param input The command line to execute.
 * @param free_value Pointer to a structure used for freeing resources.
 * Processes input, splits into commands, executes.
 * @return Return value is 1 if exit is used, else 0.
 */
int				execute_input(char ***env, char *input, t_free *free_value,
					int *exit_code);
/**
 * @brief
 * This function checks if the global signal variable g_signal is set to SIGINT.
 * If it is, it frees the memory allocated for the command arguments and the
 * command structure itself.
 * 
 * @param cmds: A pointer to the t_cmd structure containing command arguments.
 * 
 * @return 1 if the signal is SIGINT and memory is freed, 0 otherwise.
 */
int				handle_sigint(t_cmd	*cmds, int cmd_index);
// exec_utils
/**
 * Check if a string contains only space characters.
 * @param str The string to check.
 * @retval 1 The string contains at least one non-space character.
 * @retval 0 The string contains only space characters.
 */
int				check_if_only_space(char *str);
/**
 * Free an array of strings, along with the strings themselves.
 * @param args The array of strings to free.
 */
void			free_args(char **args);
/**
 * Concatenate env path with command path.
 * @param args The command path to concatenate.
 * @param envpath The environment path to concatenate.
 * @return A newly allocated string containing the concatenated paths.
 */
char			*get_cpath(char *args, char *envpath);
/**
 * @brief Get full path of a command by searching in PATH.
 * @param arg Command path to search.
 * @param path PATH env variable.
 * @return Full path of command or NULL if not found.
 */
char			*get_cmd_path(char *arg, char *path);

//////////////// FT_CMDS
// ft_cd
/**
 * @brief Change to the directory if the path is accessible.
 * @param path Directory path to check.
 */
void			check_if_accessible(char *path, int *exit_code);
/**
 * @brief Retrieves the current working directory.
 * @return Current working directory or NULL on error.
 */
char			*ft_get_current_dir(void);
/**
 * @brief Changes the current working directory.
 * @param cmd The array of command arguments.
 * @param env The environment variables.
 */
void			ft_change_dir(char **cmd, char **env, int *exit_code);
/**
 * @brief Handles the cd built-in command.
 * Changes the current directory to the given path.
 * If no path is given, changes to $HOME. If '-', changes to
 * $OLDPWD and prints it.
 * @param cmd The array of arguments of the command.
 * @param env The environment.
 * @return 1 if the command is recognized and executed, 0 otherwise.
 */
int				ft_cd(char **cmd, char ***env, int *exit_code);
// ft_echo
/**
 * Check if string contains only a character from a given start index.
 *
 * @param str The string to check.
 * @param start The starting index of the string to check from.
 * @param car The character to check for.
 *
 * @retval 1 String contains only the character from start.
 * @retval 0 Otherwise.
 */
int				check_if_only_char(char *str, int start, char car);
/**
 * ft_echo: Execute the echo command.
 *
 * Prints arguments separated by spaces. If "-n" is the first argument,
 * does not print a newline at the end.
 *
 * @param cmd: A 2D array of strings, where the first element is the command name
 * "echo", and the rest of the elements are the command arguments.
 */
void			ft_echo(char **cmd, int *exit_code);
// ft_exit
/**
 * @brief
 * check_overflow - Checks if the given string represents a number that 
 *                  overflows or underflows a long long integer.
 * 
 * This function converts the string to a long long integer using ft_strtoll.
 * It then checks if the conversion resulted in an overflow or underflow.
 * If the string does not represent a valid number, or if the number is out
 * of the range of a long long integer.
 * 
 * @param str: The string to be checked.
 *
 * @return 1 if the number overflows or underflows, 0 otherwise.
 */
int				check_overflow(const char *str);
/**
 * @brief
 * ft_alnum_exit - Validates the exit input for the `exit` command.
 * 
 * This function checks if the second argument in the `exit_input` array is a
 * valid numeric argument. If the argument is a single '-' character or contains
 * any alphabetic characters, it prints an error message to standard error and
 * sets the exit code to 2. It also frees the input string and the `exit_input`
 * array.
 *
 * @param exit_input: An array of strings containing the exit command and
 * its arguments.
 * @param input: The original input string that needs to be freed in case
 * of an error.
 * @param exit_code: A pointer to an integer where the exit code will be stored.
 * 
 * @return 1 if the argument is invalid, 0 otherwise.
 */
int				ft_alnum_exit(char **exit_input, char *input, int *exit_code);
/**
 * @brief ft_exit - Handles the exit command for the shell.
 * 
 * This function processes the exit command, parses the input, and determines
 * the appropriate exit code. It also handles errors such as non-numeric
 * arguments and too many arguments.
 * 
 * @param input: The input string containing the exit command and its arguments.
 * @param env: The environment variables.
 * @param exit_code: Pointer to the exit code to be set.
 *
 * @return 1 if the shell should exit, 0 otherwise.
 */
int				ft_exit(char *input, char **env, int *exit_code);
// ft_export
int				execute_ft_cmds_export(char **cmd, char ***env, int *exit_code);

//////////////// FT_ENV
// dup_env
/**
 * @brief Duplicate all the environment variables.
 * @param env The environment variables.
 * @return Duplicate of env.
 */
char			**dup_all_env(char **env);
// ft_change_env
/**
 * Change an environment variable's value.
 * @param env Array of environment variables.
 * @param old_env Name of the variable to change.
 * @param new_env New value for the variable.
 * If variable not found, does nothing.
 */
void			ft_change_env(char **env, char *old_env, char *new_env);
// ft_env
/**
 * @brief Find value of given environment variable in given environment.
 * @param env The environment variables.
 * @param search_env The name of the variable to search for.
 * @return Value of the variable, or NULL if not found.
 */
char			*ft_getenv(char **env, char *search_env);
/**
 * @brief Prints all environment variables in given environment.
 * @param env The environment variables.
 */
void			ft_show_env(char **env, int *exit_code);
/**
 * @brief Frees all memory associated with given environment.
 * @param env The environment variables to free.
 */
void			ft_free_env(char **env);
char			*ft_spe_getenv(char **env, char *search_env);
// ft_export_n_unset
/**
 * @brief Reallocates memory for a new environment variable.
 * @param env The pointer to the environment variable array.
 * @return The reallocated environment variable array.
 */
char			**realloc_tab(char ***env);
/**
 * @brief ft_show_export - Displays the environment variables in a sorted order.
 * 
 * This function counts the number of environment variables, creates a copy of
 * the environment array, sorts the copied array, and then prints each
 * environment variable in the format "export <variable>". Finally, it frees the
 * allocated memory for the sorted array.
 * @param env: A null-terminated array of strings representing the environment
 * variables.
 */
void			ft_show_export(char **env);
/**
 * @brief Adds/modifies an environment variable.
 * @param env Pointer to the environment array.
 * @param old_env Variable name to update/add.
 * @param new_env New value for the variable.
 */
void			ft_export(char ***env, char *old_env, char *new_env);
/**
 * @brief Removes an environment variable from the environment array.
 *
 * This function searches for the specified environment variable in the given
 * environment array and removes it if found. The environment array is updated
 * to close the gap left by the removed variable.
 *
 * @param env The environment array.
 * @param rev_env The name of the environment variable to remove.
 *
 * @note The function assumes that the environment array is NULL-terminated.
 *       If the specified environment variable is not found, the function
 *       returns without making any changes.
 */
void			ft_unset_helper(char **env, char *rev_env);
/**
 * @brief Remove an environment variable from the environment array.
 * @param env The environment variables.
 * @param rev_env The name of the variable to remove.
 */
void			ft_unset(char **env, char **rev_env, int *exit_code);
// ft_sort_array
/**
 * Copy src array to dest array.
 * @param dest Array to copy to
 * @param src Array to copy from
 */
void			copy_array(char **dest, char **src);
/**
 * Swap the value of two pointers.
 * @param a Pointer to swap
 * @param b Pointer to swap
 */
void			swap(char **a, char **b);
/**
 * Sorts an array of strings in ascending order
 * using selection sort algorithm.
 * @param array Array of strings to sort.
 */
void			sort_strings(char **array);

//////////////// PIPES_N_FDS
// fds_handler
/**
 * @brief Parses redirections in a command.
 * @param cmd The command to which to assign the file descriptors.
 * @param tokens The array of string tokens representing the command.
 * @param i The index of the current token in the tokens array.
 */
void			ft_parse_redirection(t_cmd *cmd, char **tokens, int *i);
/**
 * @brief Handles the here document input.
 * @param cmd The command to handle.
 * @param delimiter The string that signifies the end of input.
 * @param exit_code The exit code of the command.
 * @return 0 if the here document was handled successfully, otherwise -1.
 */
int				handle_here_doc(char *delimiter, int fd);
/**
 * @brief Processes the null command.
 * @param line The line to process.
 * @param pipe_fd The file descriptor of the pipe.
 * @param delimiter The string that signifies the end of input.
 * @return 0 if the delimeter was tiped, -1 if Ctrl otherwise 1.
 */
int				process_null(char *line, char *delimiter);
/**
 * @brief Opens a file, or /dev/null on failure.
 * @param path The path of the file to open.
 * @param flags The flags used to open the file (e.g., O_RDONLY).
 * @param mode The mode to use if a new file is created.
 * @return File descriptor on success, or /dev/null on failure.
 */
int				open_file(char *path, int flags, int mode);
// pipes_handler
/**
 * @brief Counts the number of command arguments in the given tokens.
 * @param tokens Array of string tokens representing the commands and arguments.
 * @param num_tokens Number of tokens in the tokens array.
 * @param start Starting index in the tokens array for the command.
 * @return The number of command arguments in the tokens array.
 */
int				ft_count_args(char **tokens, int num_tokens, int start);
/**
 * @brief Parse tokens to fill t_cmd args.
 * @param cmd Pointer to t_cmd to fill.
 * @param tokens Array of command and argument tokens.
 * @param num_tokens Size of tokens array.
 * @param start Start index in tokens for command.
 * @return Index of next token or num_tokens if last.
 */
int				ft_fill_args(t_cmd *cmd, char **tokens, int num_tokens,
					int start);
/**
 * @brief Parse and execute a pipeline of commands.
 * @param tokens Array of command and argument tokens.
 * @param num_tokens Size of tokens array.
 * @param exit_code The exit status of the last executed command.
 */
void			cleanup_pipeline(t_cmd *cmds, int count,
					int *exit_code);
/**
 * @brief Closes non-standard file descriptors for a command.
 * @param cmd The command to cleanup.
 */
void			cleanup_fds(t_cmd *cmd);
/**
 * @brief Parse and execute a pipeline of commands.
 * @param tokens Array of command and argument tokens.
 * @param num_tokens Size of tokens array.
 * @param env Environment variables.
 */
void			ft_parse_pipeline(char **tokens, int num_tokens, char **env,
					t_free *free_value);
// handle_heredoc
int				handle_here_doc_tokens(char **tokens);
void			close_here_doc(int index);

//////////////// FT_PROCESS_INPUT
// handles
/**
 * @brief handle_dollar - Handles the dollar sign ('$') in the input.
 * 
 * This function checks if the current character in the input is a dollar sign
 * ('$') and if it is not within single quotes. If so, it increments the input
 * index, calls the expand_var function to handle variable expansion, decrements
 * the input index.
 * 
 * @param s: The current state of the input processing.
 * @param t: The buffer to store the processed input.
 * @param tok: The tokens structure to store the parsed tokens.
 *
 * @return 1 if the dollar sign is handled, 0 otherwise.
 */
int				handle_dollar(t_state *s, t_buf *t, t_tokens *tok);
/**
 * @brief handle_error - Handles the special variable '$?' in the input.
 * 
 * This function checks if the current character in the input is a dollar sign
 * ('$') followed by a question mark ('?'). If so, it increments the input
 * index, checks if it is within single quotes, and inserts the value of the
 * special variable '$?' into the buffer. It converts the exit code to a string
 * using ft_itoa, inserts it into the buffer, frees the allocated memory.
 * 
 * @param s: The current state of the input processing.
 * @param t: The buffer to store the processed input.
 * @param tok: The tokens structure to store the parsed tokens.
 * @param exit_code: The exit code of the last executed command.
 *
 * @return 1 if the special variable '$?' is handled, 0 otherwise.
 */
int				handle_error(t_state *s, t_buf *t, t_tokens *tok,
					int *exit_code);
/**
 * @brief handle_space - Handles spaces and tabs in the input.
 * 
 * This function checks if the current character in the input is a space or a
 * tab and if it is not within quotes. If so, it checks if the buffer length is
 * greater than 0, duplicates the buffer content to create a new token, adds the
 * token to the tokens structure, resets the buffer length.
 * 
 * @param s: The current state of the input processing.
 * @param t: The buffer to store the processed input.
 * @param tok: The tokens structure to store the parsed tokens.
 *
 * @return 1 if the space or tab is handled, 0 otherwise.
 */
int				handle_space(t_state *s, t_buf *t, t_tokens *tok);
/**
 * @brief handle_single_quote - Handles single quotes in the input.
 * 
 * This function checks if the current character in the input is a single quote
 * (') and if it is not within double quotes. If so, it checks if the next
 * character is also a single quote, adds an empty token to the tokens
 * structure, increments the input index, and resets the quote state. If the
 * current quote state is single quotes, it resets the quote state.
 * Otherwise, it sets the quote state to single quotes.
 * 
 * @param s: The current state of the input processing.
 * @param tok: The tokens structure to store the parsed tokens.
 *
 * @return 1 if the single quote is handled, 0 otherwise.
 */
int				handle_single_quote(t_state *s, t_tokens *tok);
/**
 * @brief handle_double_quote - Handles double quotes in the input.
 * 
 * This function checks if the current character in the input is a double quote
 * (") and if it is not within single quotes. If so, it checks if the next
 * character is also a double quote, adds an empty token to the tokens structure,
 * increments the input index, and resets the quote state. If the current quote
 * state is double quotes, it resets the quote state. Otherwise, it sets the
 * quote state to double quotes.
 * 
 * @param s: The current state of the input processing.
 * @param tok: The tokens structure to store the parsed tokens.
 *
 * @return 1 if the double quote is handled, 0 otherwise.
 */
int				handle_double_quote(t_state *s, t_tokens *tok);
// preprocess_input
/**
 * @brief Inserts the value of a variable into the buffer and token list.
 * 
 * This function takes a variable value and inserts each character into the
 * buffer. If a space is encountered, the current buffer content is added as a
 * token, and the buffer is reset.
 * 
 * @param value The value of the variable to be inserted.
 * @param t The buffer structure to hold the characters.
 * @param tok The token list to hold the tokens.
 */
void			insert_var_value(char *value, t_buf *t, t_tokens *tok);
/**
 * @brief Expands a variable from the input and inserts its value.
 * 
 * This function identifies a variable name in the input, retrieves its value
 * from the environment, and inserts the value into the buffer and token list.
 * 
 * @param s The current state of the input parsing.
 * @param t The buffer structure to hold the characters.
 * @param tok The token list to hold the tokens.
 */
void			expand_var(t_state *s, t_buf *t, t_tokens *tok);
/**
 * @brief Processes a single character from the input.
 * 
 * This function handles special characters, quotes, spaces, and variable
 * expansion. It updates the buffer and token list accordingly.
 * 
 * @param s The current state of the input parsing.
 * @param t The buffer structure to hold the characters.
 * @param tok The token list to hold the tokens.
 * @param exit_code The exit code to be set in case of an error.
 */
void			process_char(t_state *s, t_buf *t, t_tokens *tok,
					int *exit_code);
/**
 * @brief Main parsing loop for the input.
 * 
 * This function iterates over the input string, processing each character and
 * updating the buffer and token list. It handles syntax errors and returns an
 * appropriate exit code.
 * 
 * @param in The input string to be parsed.
 * @param t The buffer structure to hold the characters.
 * @param tok The token list to hold the tokens.
 * @param exit_code The exit code to be set in case of an error.
 * @return int Returns 0 on success, 1 on syntax error.
 */
int				parse_loop(const char *in, t_buf *t, t_tokens *tok,
					int *exit_code);
/**
 * @brief Parses the input string into tokens.
 * 
 * This function initializes the necessary structures and calls the main
 * parsing loop. It returns the array of tokens or NULL in case of an error.
 * 
 * @param input The input string to be parsed.
 * @param env The environment variables.
 * @param exit_code The exit code to be set in case of an error.
 * @return char** Returns an array of tokens or NULL on error.
 */
char			**parse_input(const char *input, char **env, int *exit_code);
// prepocess_utils
/**
 * @brief Appends a character to the buffer.
 *
 * This function appends a character to the buffer, expanding the buffer's
 * capacity if necessary.
 *
 * @param t Pointer to the buffer structure.
 * @param c Character to append.
 */
void			append_char(t_buf *t, char c);
/**
 * @brief Appends a string to the buffer.
 *
 * This function appends a string to the buffer, character by character.
 *
 * @param t Pointer to the buffer structure.
 * @param s String to append.
 */
void			append_str(t_buf *t, const char *s);
/**
 * @brief Adds a token to the tokens array.
 *
 * This function adds a token to the tokens array, expanding the array's
 * capacity if necessary.
 *
 * @param tok Pointer to the tokens structure.
 * @param token Token to add.
 */
void			plus_token(t_tokens *tok, char *token);
/**
 * @brief Handles special characters in the input.
 *
 * This function handles special characters ('|', '<', '>') in the input,
 * adding them as tokens and resetting the buffer if necessary.
 *
 * @param s Pointer to the state structure.
 * @param t Pointer to the buffer structure.
 * @param tok Pointer to the tokens structure.
 * @return 1 if a special character was handled, 0 otherwise, -1 on error.
 */
int				handle_special_chars(t_state *s, t_buf *t, t_tokens *tok);

//////////////// SIG_N_UTILS
// sighandler
/**
 * @brief Signal handling utilities for the 21sh shell.
 * This function waits for the specified child process to change state
 * and handles the signal of the ctrl + C that may be received during this time.
 * @param sig The process ID of the child process to wait for.
 */
void			sigint_handler(int sig);
/**
 * @brief Signal handling utilities for the 21sh shell.
 * This function waits for the specified child process to change state
 * and handles the signal of the ctrl + C that may be received during this time
 * but only for the heredoc mode.
 * @param sig The process ID of the child process to wait for.
 */
void			sigint_handler_heredoc(int sig);
/**
 * @brief Sets up signal handlers based on the specified mode.
 *
 * This function configures signal handlers for SIGINT and SIGQUIT signals
 * based on the provided mode. The modes are defined as follows:
 * - mode 0: Default signal handling.
 * - mode 1: Custom signal handling for interactive mode.
 * - mode 2: Custom signal handling for heredoc mode.
 *
 * @param mode The mode to set up signal handlers for.
 */
void			setup_signals(int mode);
/**
 * @brief Handles signals and waits for a child process to change state.
 * @param pid The process ID of the child process.
 * @param exit_code Pointer to an integer where the exit code of
 * the child process will be stored.
 */
void			wait_pid_ignore_signals(pid_t pid, int *exit_code);
/**
 * @brief Waits for a child process to change state and handles signals.
 * @param pid The process ID of the child process to wait for.
 * @param exit_code Pointer to an integer where the exit code of
 * the child process will be stored.
 */
void			wait_ignore(pid_t pid, int *exit_code);
// utils
/**
 * @brief Counts the number of command arguments in the given array.
 * @param args Array of string arguments.
 * @return The number of command arguments in the array.
 */
int				count_args(char **args);
/**
 * @brief Counts the occurrences of a specific character in a string.
 * @param str The string in which to count occurrences of the character.
 * @param chars The character to count within the string.
 * @return The number of times the character appears in the string.
 */
int				count_chars(char *str, char chars);
/**
 * @brief Copies then appends strings.
 * @param dest The destination string which will be modified.
 * @param fstr The string to copy into the destination.
 * @param sstr The string to append to the destination.
 */
void			copy_then_cat(char *dest, char *fstr, char *sstr);
/**
 * @brief Duplicates a string and appends another string to it.
 * @param src Source string to duplicate.
 * @param sec_src String to append.
 * @return New string or NULL if error.
 */
char			*dup_then_cat(char *src, char *sec_src);
/**
 * @brief Checks if a string is alphanumeric.
 * @param str The string to check.
 * @return 1 if the string is alphanumeric, 0 otherwise.
 */
int				ft_stralnum(char *str);
// utils2
/**
 * @brief Concatenates three strings.
 * @param s1 First string.
 * @param s2 Second string.
 * @param s3 Third string.
 * @return The concatenated string or NULL on failure.
 */
char			*ft_strjoin_join(const char *s1, const char *s2,
					const char *s3);
/**
 * @brief Prints a string if it is a path, otherwise prints path.
 * @param path The string to print.
 */
void			print_ifpath_ornot(char *path);
/**
 * @brief Parse and execute a pipeline of commands.
 * @param tokens Array of command and argument tokens.
 * @param num_tokens Size of tokens array.
 * @return The number of commands in the pipeline.
 */
int				ft_count_first_commands(char **tokens, int num_tokens);
/**
 * @brief edit the SHLVL env variable
 * @param env the env variable
 */
void			edit_lvl(char **env);
void			handle_errno(t_pipeline_ctx *ctx, t_free *free_value,
					t_pid_struct *new_pid);

//////////////// MAIN
/**
 * @brief Print a stylized ASCII art graffiti to the console.
 */
void			print_graffiti(void);
/**
 * @brief Puts the exit code as 130 if user did a Ctrl + C or 131 if
 * Ctrl + \ in interactive mode.
 * @param exit_code The current exit code.
 */
void			sigcode(int *exit_code);
/**
 * @brief Get the relative path by replacing the home directory with '~'.
 * @param pwd The current working directory.
 * @param env The environment variables.
 * @return A newly allocated string with the relative path or original `pwd`.
 */
char			*get_relative_path(char *pwd, char **env);
/**
 * @brief Reads user input and executes it.
 * @param cwd The current working directory.
 * @param env The environment variables.
 * @param exit_status The exit status of the last executed command.
 * @return 1 if the user wants to exit the shell, else 0.
 */
int				read_lines(char *cwd, char ***env, int *exit_code);
/**
 * @brief Main entry point of the program.
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @param env The environment variables.
 * @return the exit code value.
 * Main entry point. Initializes shell,
 * reads user input in a loop until exit.
 */
int				main(int argc, char **argv, char **env);

#endif

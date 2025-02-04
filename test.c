#include "minishell.h"

void ft_execute(t_cmd *cmd)
{
	if (cmd->input_fd != STDIN_FILENO)
	{
		dup2(cmd->input_fd, STDIN_FILENO);
		close(cmd->input_fd);
	}
	if (cmd->output_fd != STDOUT_FILENO)
	{
		dup2(cmd->output_fd, STDOUT_FILENO);
		close(cmd->output_fd);
	}
	execvp(cmd->args[0], cmd->args);
	perror("execvp");
	exit(EXIT_FAILURE);
}

void ft_parse_redirection(t_cmd *cmd, char **tokens, int *i)
{
	if (ft_strcmp(tokens[*i], "<") == 0)
	{
		cmd->input_fd = open(tokens[++(*i)], O_RDONLY);
		if (cmd->input_fd < 0)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}
	}
	else if (ft_strcmp(tokens[*i], ">") == 0)
	{
		cmd->output_fd = open(tokens[++(*i)], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (cmd->output_fd < 0)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}
	}
	else if (ft_strcmp(tokens[*i], ">>") == 0)
	{
		cmd->output_fd = open(tokens[++(*i)], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (cmd->output_fd < 0)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}
	}
	else if (ft_strcmp(tokens[*i], "<<") == 0)
	{
		char *delimiter = tokens[++(*i)];
		char *line = NULL;
		int pipe_fd[2];
		pipe(pipe_fd);
		while (1)
		{
			line = readline("> ");
			if (ft_strcmp(line, delimiter) == 0)
			{
				free(line);
				break;
			}
			write(pipe_fd[1], line, strlen(line));
			write(pipe_fd[1], "\n", 1);
			free(line);
		}
		close(pipe_fd[1]);
		cmd->input_fd = pipe_fd[0];
	}
}

void ft_parse_pipeline(char **tokens, int num_tokens)
{
	int cmd_count = 0;
	int i = 0;

	while (i < num_tokens)
	{
		if (ft_strcmp(tokens[i], "|") == 0)
			cmd_count++;
		i++;
	}
	cmd_count++;

	t_cmd *cmds = malloc(cmd_count * sizeof(t_cmd));
	if (!cmds)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	i = 0;
	int cmd_index = 0;
	while (i < num_tokens)
	{
		cmds[cmd_index].input_fd = STDIN_FILENO;
		cmds[cmd_index].output_fd = STDOUT_FILENO;
		int arg_count = 0;

		int j = i;
		while (j < num_tokens && ft_strcmp(tokens[j], "|") != 0)
		{
			if (ft_strcmp(tokens[j], "<") == 0 || ft_strcmp(tokens[j], ">") == 0 || ft_strcmp(tokens[j], ">>") == 0 || ft_strcmp(tokens[j], "<<") == 0)
				j += 2;
			else
			{
				arg_count++;
				j++;
			}
		}

		cmds[cmd_index].args = malloc((arg_count + 1) * sizeof(char *));
		if (!cmds[cmd_index].args)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		int arg_index = 0;
		while (i < num_tokens && ft_strcmp(tokens[i], "|") != 0) {
			if (ft_strcmp(tokens[i], "<") == 0 || ft_strcmp(tokens[i], ">") == 0 || ft_strcmp(tokens[i], ">>") == 0 || ft_strcmp(tokens[i], "<<") == 0)
				ft_parse_redirection(&cmds[cmd_index], tokens, &i);
			else
				cmds[cmd_index].args[arg_index++] = tokens[i];
			i++;
		}
		cmds[cmd_index].args[arg_index] = NULL;
		cmd_index++;
		if (i < num_tokens && ft_strcmp(tokens[i], "|") == 0)
			i++;
	}

	int pipe_fds[2];
	int prev_pipe_read = STDIN_FILENO;

	for (int j = 0; j < cmd_count; j++)
	{
		if (j < cmd_count - 1)
		{
			pipe(pipe_fds);
			cmds[j].output_fd = pipe_fds[1];
			cmds[j + 1].input_fd = pipe_fds[0];
		}

		pid_t pid = fork();
		if (pid == 0)
		{
			if (j > 0)
				cmds[j].input_fd = prev_pipe_read;
			ft_execute(&cmds[j]);
		} else if (pid < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}

		if (j > 0)
			close(prev_pipe_read);
		if (j < cmd_count - 1)
		{
			close(pipe_fds[1]);
			prev_pipe_read = pipe_fds[0];
		}
		if (cmds[j].input_fd != STDIN_FILENO)
			close(cmds[j].input_fd);
		if (cmds[j].output_fd != STDOUT_FILENO)
			close(cmds[j].output_fd);
	}

	for (int j = 0; j < cmd_count; j++)
		wait(NULL);

	for (int j = 0; j < cmd_count; j++)
		free(cmds[j].args);
	free(cmds);
}

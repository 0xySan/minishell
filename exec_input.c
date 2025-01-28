/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:50:52 by etaquet           #+#    #+#             */
/*   Updated: 2025/01/28 16:21:03 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cpath(char *args, char *envpath)
{
	char	*cpath;

	cpath = malloc(sizeof(char) * (ft_strlen(envpath) + ft_strlen(args) + 2));
	if (!cpath)
		return (NULL);
	ft_strcpy(cpath, envpath);
	ft_strcat(cpath, "/");
	ft_strcat(cpath, args);
	return (cpath);
}

char	*get_cmd_path(char *arg, char *path)
{
	char	*cpath;
	char	**mp;
	int		i;

	i = 0;
	if (access(arg, X_OK) != -1 && count_chars(arg, '/') >= 1)
		return (ft_strdup(arg));
	mp = ft_split(path, ':');
	if (!mp)
		return (NULL);
	while (mp[i])
	{
		cpath = get_cpath(arg, mp[i]);
		if (access(cpath, X_OK) != -1 && !ft_strstr(cpath, "//"))
			return (free_args(mp), cpath);
		free(cpath);
		i++;
	}
	free_args(mp);
	return (NULL);
}

void	child_process(char **cmd, char *actual_cmd, char **envp,
			t_pidstruct *pid)
{
	pid->pid[0] = fork();
	if (pid->pid[0] == -1)
		perror("Error: Fork failed.\n");
	if (pid->pid[0] == 0)
	{
		if (access(actual_cmd, X_OK) != -1)
			execve(actual_cmd, cmd, envp);
		else
			perror("21sh");
	}
}

char	*export_util_func(char *word)
{
	int		i;
	int		j;
	char	*r_value;

	i = 0;
	j = 0;
	while (word[i] && word[i] != '=')
		i++;
	r_value = malloc((i + 1) * sizeof(char));
	if (!r_value)
		return (NULL);
	while (j < i)
	{
		r_value[j] = word[j];
		j++;
	}
	r_value[j] = '\0';
	return (r_value);
}

char	*preprocess_input(const char *input, char **env)
{
	char	*result;
	size_t	i;
	size_t	j;
	int		in_single_quote;
	int		in_double_quote;

	result = malloc(ft_strlen(input) * 2 + 1);
	if (!result)
		return (NULL);
	j = 0;
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			i++;
			continue ;
		}
		else if (input[i] == '"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			i++;
			continue ;
		}
		else if (input[i] == '$' && !in_single_quote)
		{
			size_t var_start = ++i;
			while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
				i++;
			size_t var_len = i - var_start;
			if (var_len > 0)
			{
				char var_name[var_len + 1];
				ft_strncpy(var_name, &input[var_start], var_len);
				var_name[var_len] = '\0';
				char *var_value = ft_getenv(env, var_name);
				if (var_value)
				{
					size_t value_len = ft_strlen(var_value);
					ft_strncpy(&result[j], var_value, value_len);
					j += value_len;
				}
			}
			continue;
		}
		else
			result[j++] = input[i++];
	}
	result[j] = '\0';
	if (in_single_quote || in_double_quote)
	{
		free(result);
		return (NULL);
	}
	return (result);
}

void	execute_input(char ***env, t_pidstruct *pid, char *input)
{
	char	**cmd;
	char	*cmd_path;
	char	*export_util;
	char	*parsed_input;
	
	parsed_input = preprocess_input(input, *env);
	if (!parsed_input)
		return ;
	cmd = ft_split(parsed_input, ' ');
	free(parsed_input);
	if (ft_cd(cmd, *env))
	{
		ft_change_env(*env, "_", cmd[count_args(cmd) - 1]);
		return (free_args(cmd));
	}
	if (!ft_strncmp(cmd[0], "export", 7))
	{
		if (!cmd[1])
		{
			ft_export(env, NULL, NULL);
			ft_change_env(*env, "_", cmd[count_args(cmd) - 1]);
			free_args(cmd);
		}
		else
		{
			if (!ft_strchr(cmd[1], '='))
				return (ft_change_env(*env, "_", cmd[count_args(cmd) - 1]));
			export_util = export_util_func(cmd[1]);
			ft_export(env, export_util, ft_strrchr(cmd[1], '=') + 1);
			ft_change_env(*env, "_", cmd[count_args(cmd) - 1]);
			free_args(cmd);
			free(export_util);
		}
		return ;
	}
	if (!ft_strncmp(cmd[0], "unset", 6))
	{
		ft_change_env(*env, "_", cmd[count_args(cmd) - 1]);
		ft_unset(*env, cmd[1]);
		free_args(cmd);
		return ;
	}
	if (!ft_strncmp(cmd[0], "pwd", 4))
	{
		printf("%s\n", ft_getenv(*env, "PWD"));
		return ;
	}
	if (!ft_strncmp(cmd[0], "echo", 5))
	{
		ft_change_env(*env, "_", cmd[count_args(cmd) - 1]);
		ft_echo(cmd);
		free_args(cmd);
		return ;
	}
	if (!ft_strncmp(cmd[0], "env", 4))
	{
		ft_change_env(*env, "_", cmd[count_args(cmd) - 1]);
		ft_show_env(*env);
		return ;
	}
	cmd_path = get_cmd_path(cmd[0], ft_getenv(*env, "PATH"));
	if (!cmd_path)
	{
		ft_change_env(*env, "_", cmd[count_args(cmd) - 1]);
		printf("21sh: Invalid command: %s\n", cmd[0]);
		return (free_args(cmd));
	}
	ft_change_env(*env, "_", cmd[count_args(cmd) - 1]);
	child_process(cmd, cmd_path, *env, pid);
	waitpid(pid->pid[0], NULL, 0);
	free(cmd_path);
	free_args(cmd);
}

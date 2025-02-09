/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oxy <oxy@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:50:52 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/09 21:42:48 by oxy              ###   ########.fr       */
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

static char	*get_var_name(t_parser *p, size_t *var_len)
{
	size_t	start;
	char	*name;

	p->i++;
	start = p->i;
	while (p->input[p->i] && (ft_isalnum(p->input[p->i])
			|| p->input[p->i] == '_'))
	{
		if (p->input[p->i] == '?')
			p->i++;
		p->i++;
	}
	*var_len = p->i - start;
	if (*var_len == 0 && p->input[p->i] != '?')
		return (NULL);
	name = malloc(*var_len + 1);
	if (name == NULL)
		return (NULL);
	ft_strncpy(name, &p->input[start], *var_len);
	name[*var_len] = '\0';
	return (name);
}

char	*handle_variable(t_parser *p, char **env, int *exit_status)
{
	size_t	var_len;
	char	*var_name;
	char	*var_value;
	char	*itoa_result;

	var_name = get_var_name(p, &var_len);
	if (var_name == NULL)
		return (p->result);
	var_value = ft_getenv(env, var_name);
	free(var_name);
	if (var_value)
	{
		ft_strncpy(&p->result[p->j], var_value, ft_strlen(var_value));
		p->j += ft_strlen(var_value);
	}
	else if (p->input[p->i] == '?')
	{
		itoa_result = ft_itoa(WEXITSTATUS(*exit_status));
		ft_strncpy(&p->result[p->j], itoa_result,
			ft_strlen(itoa_result) + 1);
		p->j += ft_strlen(itoa_result) + 1;
		free(itoa_result);
	}
	return (p->result);
}

static int	process_input_char(t_parser *p, char **env, int *exit_status)
{
	if (p->input[p->i] == '\'' && p->in_double == 0)
		p->in_single = !p->in_single;
	else if (p->input[p->i] == '"' && p->in_single == 0)
		p->in_double = !p->in_double;
	else if (p->input[p->i] == '$' && p->in_single == 0)
	{
		if (handle_variable(p, env, exit_status) == NULL)
			return (0);
	}
	else
		p->result[p->j++] = p->input[p->i];
	p->i++;
	return (1);
}

char	*preprocess_input(const char *input, char **env, int *exit_status)
{
	t_parser	p;

	p.input = input;
	p.i = 0;
	p.result = malloc(ft_strlen(input) * 1666666 + 1);
	if (p.result == NULL)
		return (NULL);
	p.j = 0;
	p.in_single = 0;
	p.in_double = 0;
	while (p.input[p.i])
		if (!process_input_char(&p, env, exit_status))
			return (free(p.result), NULL);
	p.result[p.j] = '\0';
	if (p.in_single || p.in_double)
		return (free(p.result), NULL);
	return (p.result);
}

int	execute_ft_cmds_export(char **cmd, char ***env)
{
	char	*export_util;

	if (!ft_strncmp(cmd[0], "export", 7))
	{
		if (!cmd[1])
		{
			ft_export(env, NULL, NULL);
		}
		else
		{
			if (!ft_strchr(cmd[1], '='))
				return (ft_change_env(*env, "_", cmd[count_args(cmd) - 1]), 1);
			export_util = export_util_func(cmd[1]);
			ft_export(env, export_util, ft_strchr(cmd[1], '=') + 1);
			free(export_util);
		}
		return (1);
	}
	return (0);
}

int	execute_ft_cmds(char **cmd, char ***env)
{
	if (ft_cd(cmd, *env))
		return (1);
	if (!ft_strncmp(cmd[0], "unset", 6))
		return (ft_unset(*env, cmd[1]), 1);
	if (!ft_strncmp(cmd[0], "pwd", 4))
		return (printf("%s\n", ft_getenv(*env, "PWD")), 1);
	if (!ft_strncmp(cmd[0], "echo", 5))
		return (ft_echo(cmd), 1);
	if (!ft_strncmp(cmd[0], "env", 4))
		return (ft_show_env(*env), 1);
	if (execute_ft_cmds_export(cmd, env))
		return (1);
	return (0);
}

void	execute_input(char ***env, char *input, int *exit_status)
{
	char	**cmds;
	char	**cmd;
	char	*parsed_input;
	int		cmd_count;

	parsed_input = preprocess_input(input, *env, exit_status);
	if (!parsed_input)
		return (free_args(cmd));
	cmds = ft_split(parsed_input, ' ');
	cmd = split_array(cmds, "<|>");
	cmd_count = count_args(cmd);
	free_args(cmds);
	free(parsed_input);
	if (ft_count_commands(cmd, cmd_count) == 1)
	{
		if (execute_ft_cmds(cmd, env))
		{
			ft_change_env(*env, "_", cmd[count_args(cmd) - 1]);
			free_args(cmd);
			return ;
		}
	}
	ft_change_env(*env, "_", cmd[count_args(cmd) - 1]);
	ft_parse_pipeline(cmd, cmd_count, *env, exit_status);
	free_args(cmd);
}

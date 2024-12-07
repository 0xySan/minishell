/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:50:52 by etaquet           #+#    #+#             */
/*   Updated: 2024/12/07 11:54:10 by etaquet          ###   ########.fr       */
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

char	*get_cmd_path(char *arg)
{
	char	*cpath;
	char	**mp;
	int		i;

	i = 0;
	if (access(arg, X_OK) != -1)
		return (ft_strdup(arg));
	mp = getpath();
	if (!mp)
		return (NULL);
	while (mp[i])
	{
		cpath = get_cpath(arg, mp[i]);
		if (access(cpath, X_OK) != -1)
			return (free_args(mp), cpath);
		free(cpath);
		i++;
	}
	free_args(mp);
	return (NULL);
}

void	child_process(char **cmd, char *actual_cmd, char **envp, t_pidstruct	*pid)
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

void	execute_input(char *cwd, char **env, t_pidstruct *pid, char *input)
{
	char	**cmd;
	char	*cmd_path;

	cmd = ft_split(input, ' ');
	if (ft_cd(cwd, cmd))
		return (free_args(cmd));
	cmd_path = get_cmd_path(cmd[0]);
	if (!cmd_path)
		return (perror("21sh"));
	if (access(cmd_path, X_OK) != -1)
	{
		child_process(cmd, cmd_path, env, pid);
		waitpid(pid->pid[0], NULL, 0);
	}
	else
		perror("21sh");
	free(cmd_path);
	free_args(cmd);
}

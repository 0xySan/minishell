/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 15:51:08 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/07 21:53:04 by hdelacou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	count_chars(char *str, char chars)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == chars)
			j++;
		i++;
	}
	return (j);
}

void	copy_then_cat(char *dest, char *fstr, char *sstr)
{
	ft_strcpy(dest, fstr);
	ft_strcat(dest, sstr);
}

char	*dup_then_cat(char *src, char *sec_src)
{
	char	*dest;
	int		len;
	int		start;

	if (!src)
		return (0);
	dest = malloc(sizeof(char) * (ft_strlen(src) + ft_strlen(sec_src) + 1));
	if (!dest)
		return (0);
	start = -1;
	while (src[++start])
		dest[start] = src[start];
	dest[start] = '\0';
	ft_strcat(dest, sec_src);
	return (dest);
}

void	wait_for_children(t_pipeline_ctx *ctx)
{
	int	i;
	int	status;

	i = 0;
	while (i < ctx->count)
		waitpid(ctx->cmds[i++].pid, &status, 0);
}

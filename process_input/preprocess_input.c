/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oxy <oxy@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 22:00:41 by oxy               #+#    #+#             */
/*   Updated: 2025/02/09 22:16:00 by oxy              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static char	*handle_variable(t_parser *p, char **env, int *exit_status)
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
		p->i--;
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
	p.result = malloc(sizeof(char)
			* (preprocess_count(input, env, exit_status) + 1));
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

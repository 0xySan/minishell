/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_count.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 22:02:37 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/10 23:47:28 by hdelacou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Extracts the name of a variable from the input string.
 * @param p Parser struct with input info.
 * @param var_len Address to store the length of the variable name.
 * @return The name of the variable or NULL if the variable is not valid.
 */
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

/**
 * Handles a variable in the input string.
 * @param p Parser struct with input info.
 * @param env Environment variables.
 * @param exit_status Exit status of last command.
 * Increments 'j' by length of variable or exit status if var is '?'.
 */
static void	handle_variable(t_parser *p, char **env, int *exit_status)
{
	size_t	var_len;
	char	*var_name;
	char	*var_value;
	char	*itoa_result;

	var_name = get_var_name(p, &var_len);
	if (var_name == NULL)
		return ;
	var_value = ft_getenv(env, var_name);
	free(var_name);
	if (var_value)
	{
		p->j += ft_strlen(var_value);
	}
	else if (p->input[p->i] == '?')
	{
		itoa_result = ft_itoa(WEXITSTATUS(*exit_status));
		p->j += ft_strlen(itoa_result) + 1;
		free(itoa_result);
	}
}

/**
 * Process a character in the input string.
 * @param p Parser struct with input info.
 * @param env Environment variables.
 * @param exit_status Exit status of last command.
 * Toggles quote flags or processes variables.
 */
static void	process_input_char(t_parser *p, char **env, int *exit_status)
{
	if (p->input[p->i] == '\'' && p->in_double == 0)
		p->in_single = !p->in_single;
	else if (p->input[p->i] == '"' && p->in_single == 0)
		p->in_double = !p->in_double;
	else if (p->input[p->i] == '$' && p->in_single == 0)
	{
		handle_variable(p, env, exit_status);
		p->i--;
	}
	else if (p->input[p->i] == '~')
		p->j += ft_strlen(ft_getenv(env, "HOME"));
	p->i++;
}

/**
 * Counts characters in a string after expanding variables.
 * @param input The string to process.
 * @param env The environment variables.
 * @param exit_status The exit status of the last command.
 * @return The number of characters in the processed string.
 */
int	preprocess_count(const char *input, char **env, int *exit_status)
{
	t_parser	p;

	p.input = input;
	p.i = 0;
	p.j = 0;
	p.in_single = 0;
	p.in_double = 0;
	while (p.input[p.i])
		process_input_char(&p, env, exit_status);
	return (p.i + p.j);
}

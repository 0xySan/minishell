/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 01:55:59 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/25 16:05:16 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	insert_var_value(char *value, t_buf *t, t_tokens *tok)
{
	int	k;

	k = 0;
	while (value && value[k])
	{
		if (value[k] == ' ')
		{
			if (t->len > 0)
			{
				plus_token(tok, strdup(t->buf));
				t->len = 0;
				t->buf[0] = '\0';
			}
		}
		else
			append_char(t, value[k]);
		k++;
	}
}

void	expand_var(t_state *s, t_buf *t, t_tokens *tok)
{
	char	*var_name;
	char	*value;
	int		i;
	int		j;

	i = 0;
	while (s->input[s->i + i] && (isalnum(s->input[s->i + i])
			|| s->input[s->i + i] == '_'))
		i++;
	if (i == 0)
	{
		insert_var_value("$", t, tok);
		return ;
	}
	var_name = malloc(i + 1);
	if (!var_name)
		return ;
	j = 0;
	while (s->input[s->i] && (isalnum(s->input[s->i]) || s->input[s->i] == '_'))
		var_name[j++] = s->input[s->i++];
	var_name[j] = '\0';
	value = ft_getenv(tok->env, var_name);
	free(var_name);
	insert_var_value(value, t, tok);
}

void	process_char(t_state *s, t_buf *t, t_tokens *tok, int *exit_code)
{
	if (handle_space(s, t, tok))
		return ;
	if (handle_single_quote(s, tok))
		return ;
	if (handle_double_quote(s, tok))
		return ;
	if (handle_special_chars(s, t, tok))
		return ;
	if (s->input[s->i] == '~' && s->quote == 0)
	{
		if (!ft_getenv(tok->env, "HOME"))
			return (insert_var_value(getenv("HOME"), t, tok));
		return (insert_var_value(ft_getenv(tok->env, "HOME"), t, tok));
	}
	if (handle_error(s, t, tok, exit_code))
		return ;
	if (handle_dollar(s, t, tok))
		return ;
	append_char(t, s->input[s->i]);
}

int	parse_loop(const char *in, t_buf *t, t_tokens *tok, int *exit_code)
{
	t_state	s;
	char	*token;

	s.input = in;
	s.i = 0;
	s.quote = 0;
	while (s.input[s.i])
	{
		process_char(&s, t, tok, exit_code);
		s.i++;
	}
	if (t->len > 0)
	{
		token = strdup(t->buf);
		plus_token(tok, token);
	}
	if (s.quote != 0)
	{
		ft_putstr_fd("minishell: syntax error\n", STDERR_FILENO);
		*exit_code = 2;
		return (1);
	}
	return (0);
}

char	**parse_input(const char *input, char **env, int *exit_code)
{
	t_tokens	tokens;
	t_buf		t;

	tokens.count = 0;
	tokens.cap = 10;
	tokens.arr = ft_calloc((tokens.cap * 2), sizeof(char *));
	tokens.env = env;
	t.cap = strlen(input) + 1;
	t.len = 0;
	t.buf = malloc(t.cap);
	t.buf[0] = '\0';
	if (parse_loop(input, &t, &tokens, exit_code))
		return (free_args(tokens.arr), NULL);
	free(t.buf);
	tokens.arr[tokens.count] = NULL;
	return (tokens.arr);
}

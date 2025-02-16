/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 01:55:59 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/16 02:19:08 by hdelacou         ###   ########.fr       */
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
	char	var_name[256];
	char	*value;

	extract_var_name(s, var_name);
	value = ft_getenv(tok->env, var_name);
	insert_var_value(value, t, tok);
}

void	process_char(t_state *s, t_buf *t, t_tokens *tok)
{
	if (handle_space(s, t, tok) == 1)
		return ;
	if (handle_single_quote(s) == 1)
		return ;
	if (handle_double_quote(s) == 1)
		return ;
	if (s->input[s->i] == '~' && s->quote == 0)
		return (insert_var_value(getenv("HOME"), t, tok));
	if (handle_error(s, t, tok) == 1)
		return ;
	if (handle_dollar(s, t, tok) == 1)
		return ;
	append_char(t, s->input[s->i]);
}

void	parse_loop(const char *in, t_buf *t, t_tokens *tok)
{
	t_state	s;
	char	*token;

	s.input = in;
	s.i = 0;
	s.quote = 0;
	while (s.input[s.i])
	{
		process_char(&s, t, tok);
		s.i++;
	}
	if (t->len > 0)
	{
		token = strdup(t->buf);
		plus_token(tok, token);
	}
}

char	**parse_input(const char *input, char **env, int *exit_status)
{
	t_tokens	tokens;
	t_buf		t;

	tokens.count = 0;
	tokens.cap = 10;
	tokens.arr = malloc(tokens.cap * sizeof(char *));
	tokens.env = env;
	tokens.exit_status = exit_status;
	t.cap = strlen(input) + 1;
	t.len = 0;
	t.buf = malloc(t.cap);
	t.buf[0] = '\0';
	parse_loop(input, &t, &tokens);
	free(t.buf);
	tokens.arr[tokens.count] = NULL;
	return (tokens.arr);
}

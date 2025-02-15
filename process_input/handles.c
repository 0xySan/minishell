/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 06:32:17 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/15 06:39:17 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_dollar(t_state *s, t_buf *t, t_tokens *tok)
{
	if (s->input[s->i] == '$' && s->quote != 1)
	{
		s->i++;
		expand_var(s, t, tok);
		s->i--;
		return (1);
	}
	return (0);
}

int	handle_error(t_state *s, t_buf *t, t_tokens *tok)
{
	char	*itoa_result;

	if (s->input[s->i] == '$' && s->input[s->i + 1] == '?')
	{
		s->i++;
		if (s->quote == 1)
			return (insert_var_value("$?", t, tok), 1);
		itoa_result = ft_itoa(WEXITSTATUS(*(tok->exit_status)));
		insert_var_value(itoa_result, t, tok);
		free(itoa_result);
		return (1);
	}
	return (0);
}

int	handle_space(t_state *s, t_buf *t, t_tokens *tok)
{
	char	c;
	char	*token;

	c = s->input[s->i];
	if (s->quote == 0 && c == ' ')
	{
		if (t->len > 0)
		{
			token = ft_strdup(t->buf);
			plus_token(tok, token);
			t->len = 0;
			t->buf[0] = '\0';
		}
		return (1);
	}
	return (0);
}

int	handle_single_quote(t_state *s)
{
	char	c;

	c = s->input[s->i];
	if (c == '\'' && s->quote != 2)
	{
		if (s->quote == 1)
			s->quote = 0;
		else
			s->quote = 1;
		return (1);
	}
	return (0);
}

int	handle_double_quote(t_state *s)
{
	char	c;

	c = s->input[s->i];
	if (c == '"' && s->quote != 1)
	{
		if (s->quote == 2)
			s->quote = 0;
		else
			s->quote = 2;
		return (1);
	}
	return (0);
}

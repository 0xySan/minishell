/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 06:35:33 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/18 08:08:12 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	append_char(t_buf *t, char c)
{
	if (t->len + 1 >= t->cap)
	{
		t->cap = t->cap * 2;
		t->buf = ft_realloc(t->buf, t->cap);
	}
	t->buf[t->len] = c;
	t->len++;
	t->buf[t->len] = '\0';
}

void	append_str(t_buf *t, const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		append_char(t, s[i]);
		i++;
	}
}

void	plus_token(t_tokens *tok, char *token)
{
	if (tok->count >= tok->cap)
	{
		tok->cap = tok->cap * 2;
		tok->arr = ft_realloc(tok->arr, tok->cap * sizeof(char *));
	}
	tok->arr[tok->count] = token;
	tok->count++;
}

int	handle_special_chars(t_state *s, t_buf *t, t_tokens *tok)
{
	char	*tmp;
	int		start;
	int		op_len;

	if (s->quote == 0 && (s->input[s->i] == '|'
			|| s->input[s->i] == '<' || s->input[s->i] == '>'))
	{
		if (t->len > 0)
		{
			plus_token(tok, ft_strdup(t->buf));
			t->len = 0;
			t->buf[0] = '\0';
		}
		start = s->i;
		while (s->input[s->i] && s->input[s->i] == s->input[start])
			s->i++;
		op_len = s->i - start;
		tmp = ft_substr(s->input, start, op_len);
		if (!tmp)
			return (-1);
		plus_token(tok, tmp);
		s->i--;
		return (1);
	}
	return (0);
}

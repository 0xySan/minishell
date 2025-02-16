/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 06:35:33 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/16 03:12:53 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	extract_var_name(t_state *s, char *var_name)
{
	int	j;

	j = 0;
	while (s->input[s->i] && (isalnum(s->input[s->i]) || s->input[s->i] == '_'))
	{
		if (j < 255)
			var_name[j++] = s->input[s->i];
		s->i++;
	}
	var_name[j] = '\0';
}

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

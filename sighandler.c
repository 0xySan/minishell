/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sighandler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:44:57 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/16 07:53:51 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief SIGINT handler for readline.
 * Called when SIGINT is received (e.g., Ctrl+C).
 * Prints a newline, clears the current line,
 * and updates the prompt.
 * @param sig The ignored signal.
 */
void	sigint_handler(int sig)
{
	(void)sig;
	printf("\e[1m\x1B[31m\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_exit_status = 130 << 8;
}

/**
 * @brief SIGQUIT handler for readline.
 * Called when SIGQUIT is received (e.g., Ctrl+\).
 * Prints the prompt without
 * clearing the current line.
 * @param sig The ignored signal.
 */
void	sigquit_handler(int sig)
{
	(void)sig;
	printf("\e[1m\x1B[33mMINISHELL \e[1m\x1B[31m");
	rl_on_new_line();
	rl_redisplay();
}

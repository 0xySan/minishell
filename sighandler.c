/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sighandler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelacou <hdelacou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:44:57 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/17 22:33:13 by hdelacou         ###   ########.fr       */
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
	printf("\n");
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
	rl_on_new_line();
	rl_redisplay();
}

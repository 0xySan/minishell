/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sighandler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:44:57 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/25 13:31:03 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_signal = 0;

void	sigint_handler_heredoc(int sig)
{
	g_signal = sig;
	close(STDIN_FILENO);
}

void	setup_signals(int mode)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	if (mode == 0)
	{
		sa.sa_handler = SIG_DFL;
		sigaction(SIGQUIT, &sa, NULL);
	}
	else if (mode == 1)
	{
		sa.sa_flags = SA_RESTART;
		sa.sa_handler = sigint_handler;
		sigaction(SIGINT, &sa, NULL);
		sa.sa_handler = SIG_IGN;
		sigaction(SIGQUIT, &sa, NULL);
	}
	else if (mode == 2)
	{
		sa.sa_handler = sigint_handler_heredoc;
		sigaction(SIGINT, &sa, NULL);
		sa.sa_handler = SIG_IGN;
		sigaction(SIGQUIT, &sa, NULL);
	}
}

void	sigint_handler(int sig)
{
	g_signal = sig;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	wait_pid_ignore_signals(pid_t pid, int *exit_code)
{
	struct sigaction	ignore_sig;
	struct sigaction	old_sigint;
	struct sigaction	old_sigquit;

	ignore_sig.sa_handler = SIG_IGN;
	sigemptyset(&ignore_sig.sa_mask);
	ignore_sig.sa_flags = 0;
	sigaction(SIGINT, &ignore_sig, &old_sigint);
	sigaction(SIGQUIT, &ignore_sig, &old_sigquit);
	waitpid(pid, exit_code, WUNTRACED);
	sigaction(SIGINT, &old_sigint, NULL);
	sigaction(SIGQUIT, &old_sigquit, NULL);
}

void	wait_ignore(pid_t pid, int *exit_code)
{
	wait_pid_ignore_signals(pid, exit_code);
	if (WIFSIGNALED(*exit_code))
	{
		g_signal = WTERMSIG(*exit_code);
		if (g_signal == SIGQUIT)
			ft_dprintf(2, "Quit (core dumped)\n");
		else if (g_signal == SIGINT)
			ft_dprintf(2, "\n");
	}
}

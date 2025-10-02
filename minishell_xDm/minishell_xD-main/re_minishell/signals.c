/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: didris <didris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:13:04 by didris            #+#    #+#             */
/*   Updated: 2025/09/29 22:45:00 by didris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

volatile sig_atomic_t	g_sigint_received = 0;

void	signal_handler(int signo)
{
	(void)signo;
	g_sigint_received = 1;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	child_sig_handler(int sig)
{
	(void)sig;
	g_sigint_received = 1;
	write(1, "\n", 1);
}

void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_child_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = child_sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

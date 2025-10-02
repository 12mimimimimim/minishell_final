/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 20:16:35 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/29 21:15:42 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>

extern volatile sig_atomic_t	g_sigint_received;

void	signal_handler(int signo);
void	setup_signals(void);
void	setup_child_signals(void);
void	rl_replace_line(const char *text, int clear_undo);
void	rl_clear_history(void);
void	heredoc_sig_handler(int sig);

#endif

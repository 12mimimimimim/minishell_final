/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:13:47 by didris            #+#    #+#             */
/*   Updated: 2025/09/29 23:00:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_l_child(t_ast *left, int *pipe_fds, t_env **env, t_clean *clean)
{
	int	code;

	setup_child_signals();
	close(pipe_fds[0]);
	dup2(pipe_fds[1], 1);
	close(pipe_fds[1]);
	code = execute(left, &env, clean);
	close_fds();
	clean_child_exit(clean, code);
}

void	pipe_r_child(t_ast *right, int *pipe_fds, t_env **env, t_clean *clean)
{
	int	code;

	setup_child_signals();
	close(pipe_fds[1]);
	dup2(pipe_fds[0], 0);
	close(pipe_fds[0]);
	code = execute(right, &env, clean);
	close_fds();
	clean_child_exit(clean, code);
}

int	pipe_parent(int l_pid, int r_pid, int *pipe_fds)
{
	int	status;
	int	code;

	close(pipe_fds[0]);
	close(pipe_fds[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(l_pid, &status, 0);
	waitpid(r_pid, &status, 0);
	if (WIFEXITED(status))
		code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		code = 128 + WTERMSIG(status);
	else
		code = 1;
	signal(SIGINT, signal_handler);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	return (code);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:13:47 by didris            #+#    #+#             */
/*   Updated: 2025/09/26 13:22:25 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	pipe_l_child(t_ast *left, int *pipe_fds, t_env **env, t_clean *clean)
{
	int		code;
	t_ast	*cmd;

	if (validate_redirs(left) != 0)
		clean_child_exit(clean, 1);
	setup_child_signals();
	close(pipe_fds[0]);
	dup2(pipe_fds[1], 1);
	close(pipe_fds[1]);
	if (left && (left->type >= AST_REDIR_IN && left->type <= AST_HEREDOC))
	{
		code = apply_redir(left, env, clean);
		if (code != 0)
			clean_child_exit(clean, code);
		cmd = find_cmd_node(left);
		if (cmd)
			code = execute(cmd, &env, clean);
		else
			code = 0;
	}
	else
		code = execute(left, &env, clean);
	clean_child_exit(clean, code);
}

void	pipe_r_child(t_ast *right, int *pipe_fds, t_env **env, t_clean *clean)
{
	int		code;
	t_ast	*cmd;

	if (validate_redirs(right) != 0)
		clean_child_exit(clean, 1);
	setup_child_signals();
	close(pipe_fds[1]);
	dup2(pipe_fds[0], 0);
	close(pipe_fds[0]);
	if (right && (right->type >= AST_REDIR_IN && right->type <= AST_HEREDOC))
	{
		code = apply_redir(right, env, clean);
		if (code != 0)
			clean_child_exit(clean, code);
		cmd = find_cmd_node(right);
		if (cmd)
			code = execute(cmd, &env, clean);
		else
			code = 0;
	}
	else
		code = execute(right, &env, clean);
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

void	pipe_cleanup(t_ast *right, t_ast *left, int *pipe_fds, char *error)
{
	if (pipe_fds)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
	close_tree_heredocs(left);
	close_tree_heredocs(right);
	if (error)
		perror(error);
}

int	handle_pipe_hd(t_ast *left, t_ast *right, t_env **env, t_clean *clean)
{
	int	last_fd;

	last_fd = -1;
	if (mult_heredoc(left, env, &last_fd, clean) < 0)
		return (-1);
	last_fd = -1;
	if (mult_heredoc(right, env, &last_fd, clean) < 0)
	{
		close_tree_heredocs(left);
		return (-1);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:37:28 by didris            #+#    #+#             */
/*   Updated: 2025/09/29 20:00:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "expander.h"

static int	hd_loop_helper(char **line, char *delim, char *trimmed_delim)
{
	size_t	len;

	if (g_sigint_received)
	{
		if (*line)
			free(*line);
		return (130);
	}
	if (!*line)
	{
		write(2, "minishell: warning: here-document ", 34);
		write(2, "delimited by end-of-file (wanted `", 34);
		write(2, delim, ft_strlen(delim));
		write(2, "`)\n", 3);
		return (1);
	}
	len = ft_strlen(*line);
	if (len > 0 && (*line)[len - 1] == '\n')
		(*line)[len - 1] = '\0';
	if (ft_strncmp(*line, trimmed_delim, ft_strlen(trimmed_delim) + 1) == 0)
	{
		free(*line);
		return (1);
	}
	return (0);
}

static int	hd_child_process(char *delim, t_env **env, int write_fd)
{
	char	*line;
	char	*expanded_line;
	char	*trimmed_delim;
	int		code;

	line = NULL;
	trimmed_delim = process_mixed_args(delim, env, 1);
	g_sigint_received = 0;
	setup_child_signals();
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		code = hd_loop_helper(&line, delim, trimmed_delim);
		if (code != 0)
			break ;
		expanded_line = process_heredoc_line(line, is_quoted(delim) == 0, env);
		write(write_fd, expanded_line, ft_strlen(expanded_line));
		write(write_fd, "\n", 1);
		free(expanded_line);
		free(line);
	}
	get_next_line(-1);
	free(trimmed_delim);
	return (code);
}

static int	hd_parent_process(pid_t pid, int read_fd)
{
	int				status;
	struct termios	or_termios;

	if (tcgetattr(STDIN_FILENO, &or_termios) == -1)
		return (perror("tcgetattr"), close(read_fd), -1);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		if (tcsetattr(STDIN_FILENO, TCSANOW, &or_termios) == -1)
			perror("tcsetattr");
		signal(SIGINT, signal_handler);
		close(read_fd);
		g_sigint_received = 1;
		return (-1);
	}
	signal(SIGINT, signal_handler);
	return (read_fd);
}

static int	do_heredoc(char *delim, t_env **env, t_clean *clean)
{
	int		pipe_fds[2];
	pid_t	pid;
	int		code;

	if (pipe(pipe_fds) == -1)
		return (perror("pipe"), -1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		close(pipe_fds[0]);
		code = hd_child_process(delim, env, pipe_fds[1]);
		close(pipe_fds[1]);
		close_fds();
		clean_child_exit(clean, code);
	}
	close(pipe_fds[1]);
	return (hd_parent_process(pid, pipe_fds[0]));
}

int	mult_heredoc(t_ast *node, t_env **env, int *last_fd, t_clean *clean)
{
	int	fd;

	if (!node || !(node->type >= AST_REDIR_IN && node->type <= AST_HEREDOC))
		return (0);
	if (mult_heredoc(node->left, env, last_fd, clean) < 0)
		return (-1);
	if (node->type == AST_HEREDOC)
	{
		if (g_sigint_received)
			return (close_last_fd(last_fd), -1);
		fd = do_heredoc(node->filename, env, clean);
		if (fd < 0)
			return (close_last_fd(last_fd), -1);
		close_last_fd(last_fd);
		*last_fd = fd;
	}
	return (0);
}

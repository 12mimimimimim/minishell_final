/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:37:28 by didris            #+#    #+#             */
/*   Updated: 2025/10/01 18:59:29 by marvin           ###   ########.fr       */
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
		return (0);
	}
	return (-1);
}

static int	hd_input_handler(char *delim, t_env **env, int write_fd)
{
	char	*line;
	char	*expanded_line;
	char	*trimmed_delim;
	int		code;

	line = NULL;
	trimmed_delim = process_mixed_args(delim, env, 1, NULL);
	g_sigint_received = 0;
	setup_child_signals();
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		code = hd_loop_helper(&line, delim, trimmed_delim);
		if (code != -1)
			break ;
		expanded_line = process_heredoc_line(line, is_quoted(delim) == 0, env);
		ft_putendl_fd(expanded_line, write_fd);
		free(expanded_line);
		free(line);
	}
	get_next_line(-1);
	free(trimmed_delim);
	return (code);
}

static int	do_heredoc(char *delim, t_env **env)
{
	int		pipe_fds[2];
	int		code;

	if (pipe(pipe_fds) == -1)
		return (perror("pipe"), -1);
	code = hd_input_handler(delim, env, pipe_fds[1]);
	close(pipe_fds[1]);
	if (code == 130)
	{
		close(pipe_fds[0]);
		g_sigint_received = 1;
		return (-1);
	}
	else if (code != 0)
	{
		close(pipe_fds[0]);
		return (-1);
	}
	return (pipe_fds[0]);
}

int	mult_heredoc(t_ast *node, t_env **env, int *last_fd, t_clean *clean)
{
	int	fd;

	if (!node)
		return (0);
	if (mult_heredoc(node->left, env, last_fd, clean) < 0)
		return (-1);
	if (mult_heredoc(node->right, env, last_fd, clean) < 0)
		return (-1);
	if (node->type == AST_HEREDOC)
	{
		if (g_sigint_received)
			return (close_fd(last_fd), -1);
		if (node->heredoc_fd < 0)
		{
			fd = do_heredoc(node->filename, env);
			if (fd < 0)
				return (close_fd(last_fd), -1);
			node->heredoc_fd = fd;
		}
		close_fd(last_fd);
		if (last_fd)
			*last_fd = node->heredoc_fd;
	}
	return (0);
}

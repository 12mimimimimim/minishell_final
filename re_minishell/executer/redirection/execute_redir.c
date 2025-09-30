/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 22:49:16 by didris            #+#    #+#             */
/*   Updated: 2025/09/27 13:38:24 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static int	handle_fd(char *filename, int flags, int target_fd, int hd_fd)
{
	int	fd;

	fd = open(filename, flags, 0644);
	if (fd < 0)
	{
		perror(filename);
		if (hd_fd >= 0)
			close(hd_fd);
		return (1);
	}
	if (dup2(fd, target_fd) < 0)
	{
		perror("dup2");
		close(fd);
		if (hd_fd >= 0)
			close(hd_fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	apply_single_redir(t_ast *cur, t_ast *node, int hd_fd)
{
	if (!cur->filename)
	{
		write(2, "minishell: missing filename for redirection\n", 44);
		if (hd_fd >= 0)
			close(hd_fd);
		return (1);
	}
	if (cur->type == AST_REDIR_OUT && cur == find_last_output_redir(node))
		return (handle_fd(cur->filename, O_CREAT | O_WRONLY | O_TRUNC,
				STDOUT_FILENO, hd_fd));
	if (cur->type == AST_REDIR_APPEND && cur == find_last_output_redir(node))
		return (handle_fd(cur->filename, O_CREAT | O_WRONLY | O_APPEND,
				STDOUT_FILENO, hd_fd));
	if (cur->type == AST_REDIR_IN && cur == find_last_input_redir(node))
		return (handle_fd(cur->filename, O_RDONLY, STDIN_FILENO, hd_fd));
	if (cur->type == AST_HEREDOC && cur == find_last_input_redir(node))
	{
		if (dup2(hd_fd, STDIN_FILENO) < 0)
		{
			perror("dup2");
			close(hd_fd);
			return (1);
		}
	}
	return (0);
}

int	apply_redir(t_ast *node, t_env **env, t_clean *clean)
{
	t_ast	*cur;
	int		hd_fd;
	int		code;

	cur = node;
	hd_fd = -1;
	code = 0;
	if (check_input_files(node) < 0)
		return (1);
	if (mult_heredoc(node, env, &hd_fd, clean) < 0)
		return (130);
	while (cur && (cur->type >= AST_REDIR_IN && cur->type <= AST_HEREDOC))
	{
		code = apply_single_redir(cur, node, hd_fd);
		if (code != 0)
		{
			if (hd_fd >= 0)
				close(hd_fd);
			return (code);
		}
		cur = cur->left;
	}
	if (hd_fd >= 0)
		close(hd_fd);
	return (0);
}

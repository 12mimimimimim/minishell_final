/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 22:49:16 by didris            #+#    #+#             */
/*   Updated: 2025/10/01 18:58:25 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static int	handle_fd(char *filename, int flags, int target_fd)
{
	int	fd;

	fd = open(filename, flags, 0644);
	if (fd < 0)
		return (1);
	if (dup2(fd, target_fd) < 0)
	{
		perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	apply_single_redir(t_ast *cur, t_ast *node)
{
	if (!cur->filename)
	{
		write(2, "minishell: missing filename for redirection\n", 44);
		return (1);
	}
	if (cur->type == AST_REDIR_OUT && cur == find_last_output_redir(node))
		return (handle_fd(cur->filename, O_CREAT | O_WRONLY | O_TRUNC,
				STDOUT_FILENO));
	if (cur->type == AST_REDIR_APPEND && cur == find_last_output_redir(node))
		return (handle_fd(cur->filename, O_CREAT | O_WRONLY | O_APPEND,
				STDOUT_FILENO));
	if (cur->type == AST_REDIR_IN && cur == find_last_input_redir(node))
		return (handle_fd(cur->filename, O_RDONLY, STDIN_FILENO));
	if (cur->type == AST_HEREDOC && cur == find_last_input_redir(node))
	{
		if (cur->heredoc_fd >= 0 && dup2(cur->heredoc_fd, STDIN_FILENO) < 0)
		{
			perror("dup2");
			return (1);
		}
		close(cur->heredoc_fd);
	}
	return (0);
}

static int	heredocs_processed(t_ast *node)
{
	if (!node)
		return (0);
	if (node->type == AST_HEREDOC && node->heredoc_fd >= 0)
		return (1);
	return (heredocs_processed(node->left)
		|| heredocs_processed(node->right));
}

int	apply_redir(t_ast *node, t_env **env, t_clean *clean)
{
	t_ast	*cur;
	int		hd_fd;
	int		code;

	cur = node;
	hd_fd = -1;
	if (!heredocs_processed(node))
	{
		if (mult_heredoc(node, env, &hd_fd, clean) < 0)
			return (130);
	}
	while (cur && (cur->type >= AST_REDIR_IN && cur->type <= AST_HEREDOC))
	{
		code = apply_single_redir(cur, node);
		if (code != 0)
		{
			close_fd(&hd_fd);
			return (code);
		}
		cur = cur->left;
	}
	close_fd(&hd_fd);
	return (0);
}

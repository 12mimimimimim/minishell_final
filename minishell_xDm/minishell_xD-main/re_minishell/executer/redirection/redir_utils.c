/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:55:05 by didris            #+#    #+#             */
/*   Updated: 2025/10/02 17:02:57 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

t_ast	*find_last_input_redir(t_ast *node)
{
	t_ast	*last;

	if (!node)
		return (NULL);
	last = find_last_input_redir(node->left);
	if (node->type == AST_REDIR_IN || node->type == AST_HEREDOC)
		return (node);
	return (last);
}

t_ast	*find_last_output_redir(t_ast *node)
{
	t_ast	*last;

	if (!node)
		return (NULL);
	last = find_last_output_redir(node->left);
	if (node->type == AST_REDIR_OUT || node->type == AST_REDIR_APPEND)
		return (node);
	return (last);
}

void	close_tree_heredocs(t_ast *node)
{
	if (!node)
		return ;
	close_tree_heredocs(node->left);
	close_tree_heredocs(node->right);
	if (node->type == AST_HEREDOC && node->heredoc_fd >= 0)
	{
		close(node->heredoc_fd);
		node->heredoc_fd = -1;
	}
}

int	validate_redirs(t_ast *node)
{
	int	tmp_fd;

	tmp_fd = -1;
	if (!node)
		return (0);
	if (validate_redirs(node->left) != 0 || validate_redirs(node->right) != 0)
		return (1);
	if (node->type >= AST_REDIR_IN && node->type <= AST_REDIR_APPEND)
	{
		if (!node->filename)
			return (1);
		if (node->type == AST_REDIR_OUT)
			tmp_fd = open(node->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (node->type == AST_REDIR_APPEND)
			tmp_fd = open(node->filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else if (node->type == AST_REDIR_IN)
			tmp_fd = open(node->filename, O_RDONLY);
		if (tmp_fd < 0)
			return (perror(node->filename), 1);
		close(tmp_fd);
	}
	return (validate_redirs(node->right));
}

void	close_fd(int *last_fd)
{
	if (last_fd && *last_fd >= 0)
		close(*last_fd);
	if (last_fd)
		*last_fd = -1;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:55:05 by didris            #+#    #+#             */
/*   Updated: 2025/09/26 12:58:37 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

t_ast	*find_last_input_redir(t_ast *node)
{
	if (!node)
		return (NULL);
	if (node->type == AST_REDIR_IN || node->type == AST_HEREDOC)
		return (node);
	return (find_last_input_redir(node->left));
}

t_ast	*find_last_output_redir(t_ast *node)
{
	if (!node)
		return (NULL);
	if (node->type == AST_REDIR_OUT || node->type == AST_REDIR_APPEND)
		return (node);
	return (find_last_output_redir(node->left));
}

int	check_input_files(t_ast *node)
{
	int	fd;

	if (!node || !(node->type >= AST_REDIR_IN && node->type <= AST_HEREDOC))
		return (0);
	if (check_input_files(node->left) < 0)
		return (-1);
	if (node->type == AST_REDIR_IN)
	{
		fd = open(node->filename, O_RDONLY);
		if (fd < 0)
		{
			perror(node->filename);
			return (-1);
		}
		close(fd);
	}
	return (0);
}

void	close_last_fd(int *last_fd)
{
	if (*last_fd >= 0)
		close(*last_fd);
	*last_fd = -1;
}

int	first_pass_open_redirs(t_ast *node)
{
	int	tmp_fd;
	int	ret;

	if (!node || node->type < AST_REDIR_IN || node->type > AST_HEREDOC)
		return (0);
	ret = first_pass_open_redirs(node->left);
	if (ret != 0)
		return (ret);
	if (!node->filename)
		return (write(2, "minishell: missing filename for redirection\n", 45),
			1);
	if (node->type == AST_REDIR_OUT)
		tmp_fd = open(node->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (node->type == AST_REDIR_APPEND)
		tmp_fd = open(node->filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (node->type == AST_REDIR_IN)
		tmp_fd = open(node->filename, O_RDONLY);
	else
		tmp_fd = -1;
	if (tmp_fd < 0)
		return (perror(node->filename), 1);
	if (tmp_fd >= 0)
		close(tmp_fd);
	return (0);
}

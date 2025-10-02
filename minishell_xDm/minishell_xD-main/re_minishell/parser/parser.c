/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 20:14:17 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 20:14:17 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_ast	*ast_initializer(t_ast_node_type which_type)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = which_type;
	node->argv = NULL;
	node->left = NULL;
	node->right = NULL;
	node->filename = NULL;
	node->heredoc_fd = -1;
	return (node);
}

t_ast	*parse_pipeline(t_ast *left_node, t_token **tokens)
{
	t_ast	*right_node;
	t_ast	*pipe_node;

	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		right_node = handle_command_or_redirection(tokens);
		if (!right_node)
			return (free_ast(left_node), NULL);
		pipe_node = ast_initializer(AST_PIPE);
		if (!pipe_node)
			return (free_ast(left_node), free_ast(right_node), NULL);
		pipe_node->left = left_node;
		pipe_node->right = right_node;
		left_node = pipe_node;
	}
	return (left_node);
}

t_ast	*parser(t_token **tokens)
{
	t_ast	*left_node;

	if (!tokens || !*tokens)
		return (NULL);
	if ((*tokens)->type == TOKEN_PIPE || (*tokens)->type == TOKEN_EOF)
		return (ft_putstr_fd("-bash: syntax error near \
            unexpected token `|'\n", 2), NULL);
	left_node = handle_command_or_redirection(tokens);
	if (!left_node)
		return (NULL);
	return (parse_pipeline(left_node, tokens));
}

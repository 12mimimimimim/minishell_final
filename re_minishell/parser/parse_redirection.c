/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 20:14:08 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 20:14:08 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_ast	*grab_filename(t_token **tokens)
{
	t_ast	*redirection_ast;
	t_token	*redirection_token;

	redirection_token = *tokens;
	(*tokens) = (*tokens)->next;
	if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
		return (NULL);
	redirection_ast = ast_initializer(redirection_token->type
			+ AST_REDIR_IN - TOKEN_REDIR_IN);
	if (!redirection_ast)
		return (NULL);
	redirection_ast->filename = ft_strdup((*tokens)->value);
	if (!redirection_ast->filename)
	{
		free_ast(redirection_ast);
		return (NULL);
	}
	*tokens = (*tokens)->next;
	return (redirection_ast);
}

t_ast	*process_redirections(t_token **tokens, t_ast *left_node)
{
	t_ast	*redirection_found;

	while (*tokens && ((*tokens)->type == TOKEN_WORD
			|| ((*tokens)->type >= TOKEN_REDIR_IN
				&& (*tokens)->type <= TOKEN_HEREDOC)))
	{
		if ((*tokens)->type == TOKEN_WORD)
			*tokens = (*tokens)->next;
		else if ((*tokens)->type >= TOKEN_REDIR_IN
			&& (*tokens)->type <= TOKEN_HEREDOC)
		{
			redirection_found = grab_filename(tokens);
			if (!redirection_found)
			{
				if (left_node)
					free_ast(left_node);
				return (NULL);
			}
			redirection_found->left = left_node;
			left_node = redirection_found;
		}
		else
			break ;
	}
	return (left_node);
}

t_ast	*handle_command_or_redirection(t_token **tokens)
{
	t_ast	*left_node;
	char	**commands;

	left_node = NULL;
	commands = collect_commands(tokens);
	if (commands)
	{
		left_node = ast_initializer(AST_COMMAND);
		if (!left_node)
			return (free_argv(commands), NULL);
		left_node->argv = commands;
	}
	return (process_redirections(tokens, left_node));
}

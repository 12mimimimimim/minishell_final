/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 20:14:40 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 20:14:40 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	copy_word_to_argv(char **argv, int *i, t_token **current)
{
	argv[*i] = strdup((*current)->value);
	if (!argv[*i])
		return (FAILURE);
	(*i)++;
	*current = (*current)->next;
	return (SUCCESS);
}

void	skip_redirection_filename(t_token **current)
{
	*current = (*current)->next;
	if (*current && (*current)->type == TOKEN_WORD)
		*current = (*current)->next;
}

int	fill_argv_array(char **argv, t_token *tokens, int count)
{
	t_token	*current;
	int		i;

	current = tokens;
	i = 0;
	while (current && i < count
		&& (current->type == TOKEN_WORD
			|| (current->type >= TOKEN_REDIR_IN
				&& current->type <= TOKEN_HEREDOC)))
	{
		if (current->type == TOKEN_WORD)
		{
			if (copy_word_to_argv(argv, &i, &current) == FAILURE)
				return (cleanup_partial_argv(argv, i), FAILURE);
		}
		else
			skip_redirection_filename(&current);
	}
	argv[i] = NULL;
	return (SUCCESS);
}

char	**collect_commands(t_token **tokens)
{
	char	**collected_commands;
	int		count;

	count = count_commands(tokens);
	if (count == 0)
		return (NULL);
	collected_commands = malloc((count + 1) * sizeof(char *));
	if (!collected_commands)
		return (NULL);
	if (!fill_argv_array(collected_commands, *tokens, count))
	{
		free_argv(collected_commands);
		return (NULL);
	}
	return (collected_commands);
}

t_ast	*parse_commands(t_token **tokens)
{
	char	**commands;
	t_ast	*parse_command_ast;

	commands = collect_commands(tokens);
	if (!commands || !*commands)
	{
		free_argv(commands);
		return (NULL);
	}
	parse_command_ast = ast_initializer(AST_COMMAND);
	if (!parse_command_ast)
	{
		free_argv(commands);
		return (NULL);
	}
	parse_command_ast->argv = commands;
	return (parse_command_ast);
}

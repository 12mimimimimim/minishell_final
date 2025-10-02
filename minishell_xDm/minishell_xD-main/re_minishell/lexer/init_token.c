/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 20:06:09 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 20:06:09 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_token_type	get_token_type(char *value)
{
	if (!value)
	{
		write(2, "NO VALUE FOUND IN GET_TOKEN TYPE\n", 33);
		return (TOKEN_EOF);
	}
	if (ft_strncmp("<<", value, 2) == 0)
		return (TOKEN_HEREDOC);
	else if (ft_strncmp(">>", value, 2) == 0)
		return (TOKEN_REDIR_APPEND);
	else if (ft_strncmp("<", value, 1) == 0)
		return (TOKEN_REDIR_IN);
	else if (ft_strncmp(">", value, 1) == 0)
		return (TOKEN_REDIR_OUT);
	else if (ft_strncmp("|", value, 1) == 0)
		return (TOKEN_PIPE);
	return (TOKEN_WORD);
}

t_token	*create_token(char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = get_token_type(value);
	new_token->value = ft_strdup(value);
	new_token->next = NULL;
	if (!new_token->value)
		return (free(new_token), NULL);
	return (new_token);
}

void	add_token_to_list(t_token **first_token, t_token **last_token,
		t_token *new_token)
{
	if (*first_token == NULL)
	{
		*first_token = new_token;
		*last_token = new_token;
	}
	else
	{
		(*last_token)->next = new_token;
		*last_token = new_token;
	}
}

t_token	*tokenator(char **argv)
{
	t_token	*first_token;
	t_token	*last_token;
	t_token	*new_token;
	int		i;

	first_token = NULL;
	last_token = NULL;
	i = 0;
	while (argv[i])
	{
		new_token = create_token(argv[i]);
		if (!new_token)
		{
			free_tokens(first_token);
			return (NULL);
		}
		add_token_to_list(&first_token, &last_token, new_token);
		i++;
	}
	return (first_token);
}

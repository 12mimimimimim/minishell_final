/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 20:13:54 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 20:13:54 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	count_commands(t_token **tokens)
{
	int		count_of_commands;
	t_token	*current;

	count_of_commands = 0;
	current = *tokens;
	while (current && (current->type == TOKEN_WORD
			|| (current->type >= TOKEN_REDIR_IN
				&& current->type <= TOKEN_HEREDOC)))
	{
		if (current->type == TOKEN_WORD)
		{
			if (current->value && current->value[0] != '\0')
				count_of_commands++;
			current = current->next;
		}
		else if (current->type >= TOKEN_REDIR_IN
			&& current->type <= TOKEN_HEREDOC)
			skip_redirection_filename(&current);
		else
			break ;
	}
	return (count_of_commands);
}

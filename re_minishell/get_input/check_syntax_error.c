/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:20:43 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 19:20:43 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	check_redirection_syntax(char *read_line, int *i)
{
	int	redir_len;
	int	next_pos;

	redir_len = get_redirection_length(read_line, *i);
	next_pos = skip_whitespace(read_line, *i + redir_len);
	if (read_line[next_pos] == '\0')
		return (syntax_error("newline"), FAILURE);
	if (is_single_redirection(read_line[next_pos])
		|| is_pipe(read_line[next_pos]))
		return (syntax_error("redirection"), FAILURE);
	*i += redir_len;
	return (SUCCESS);
}

int	skip_quoted_words(char *read_line, int *i)
{
	char	quote_char;

	quote_char = read_line[*i];
	(*i)++;
	while (read_line[*i] && read_line[*i] != quote_char)
		(*i)++;
	if (read_line[*i] == quote_char)
	{
		(*i)++;
		return (SUCCESS);
	}
	return (FAILURE);
}

int	check_syntax_at_pos(char *read_line, int *i)
{
	if (is_pipe(read_line[*i]))
	{
		if (check_pipe_syntax(read_line, i) == FAILURE)
			return (FAILURE);
	}
	else if (is_single_redirection(read_line[*i]))
	{
		if (check_redirection_syntax(read_line, i) == FAILURE)
			return (FAILURE);
	}
	else
		(*i)++;
	return (SUCCESS);
}

int	check_for_bash_syntax_error(char *read_line)
{
	int	i;

	i = 0;
	if (!read_line)
		return (FAILURE);
	while (read_line[i])
	{
		if (is_single_quote(read_line[i]) || is_double_quote(read_line[i]))
		{
			if (skip_quoted_words(read_line, &i) == FAILURE)
				return (QUOTELESS);
		}
		else if (check_syntax_at_pos(read_line, &i) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

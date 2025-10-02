/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:55:55 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 19:55:55 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	check_pipe_syntax(char *read_line, int *i)
{
	int	next_pos;

	if (!is_pipe(read_line[*i]))
		return (SUCCESS);
	next_pos = skip_whitespace(read_line, *i + 1);
	if (read_line[next_pos] == '\0')
	{
		syntax_error("newline");
		return (FAILURE);
	}
	if (is_pipe(read_line[next_pos]))
	{
		syntax_error("|");
		return (FAILURE);
	}
	(*i)++;
	return (SUCCESS);
}

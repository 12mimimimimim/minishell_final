/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quoted_word.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:37:33 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 19:37:33 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_double_quote(char c)
{
	if (c == '"')
		return (SUCCESS);
	return (FAILURE);
}

int	is_single_quote(char c)
{
	if (c == '\'')
		return (SUCCESS);
	return (FAILURE);
}

int	handle_quoted_word(char *read_line, int current_i, char c)
{
	int		i;
	char	quote;

	i = current_i;
	if (c == 'S')
	{
		while (read_line[i] && (is_single_quote(read_line[i])
				|| is_double_quote(read_line[i])))
		{
			quote = read_line[i];
			i++;
			while (read_line[i] && read_line[i] != quote)
				i++;
			if (read_line[i] == quote)
				i++;
			else
				return (QUOTELESS);
		}
		return (i - 1);
	}
	return (QUOTELESS);
}

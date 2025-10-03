/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:19:50 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 19:19:50 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	skip_whitespace(char *string, int start)
{
	while (string[start] == ' ' || string[start] == '\t')
		start++;
	return (start);
}

int	is_single_redirection(char c)
{
	if (c == '<' || c == '>')
		return (SUCCESS);
	return (FAILURE);
}

int	get_redirection_length(char *string, int i)
{
	if (string[i] == string[i + 1])
		return (2);
	return (1);
}

int	is_pipe(char c)
{
	if (c == '|')
		return (SUCCESS);
	return (FAILURE);
}

void	syntax_error(char *which_token)
{
	if (ft_strncmp(which_token, "newline", ft_strlen(which_token)) == 0)
		write(2, "-bash: syntax error near unexpected token `newline'\n", 52);
	else
	{
		write(2, "-bash: syntax error near unexpected token `", 43);
		write(2, which_token, ft_strlen(which_token));
		write(2, "'\n", 2);
	}
}

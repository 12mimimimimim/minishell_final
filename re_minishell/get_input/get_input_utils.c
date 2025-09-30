/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_input_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:22:58 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 19:22:58 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	print_error(char *code)
{
	if (ft_strncmp(code, "QUOTELESS", 10) == 0)
		write(2, "the program doesn't support unclosed quotes :( \n", 48);
	return (FAILURE);
}

int	count_quoted_word(char *read_line, int *i, int *total_word_count)
{
	if (is_double_quote(read_line[*i]) || is_single_quote(read_line[*i]))
	{
		(*total_word_count)++;
		*i = handle_quoted_word(read_line, *i, 'S');
		if (*i == QUOTELESS)
			return (FAILURE);
		return (SUCCESS);
	}
	return (QUOTELESS);
}

int	process_regular_word(char *read_line, int i,
		char delimiter, int *in_word)
{
	if (read_line[i] != delimiter && *in_word == 0)
	{
		*in_word = 1;
		return (SUCCESS);
	}
	else if (read_line[i] == delimiter)
		*in_word = 0;
	return (0);
}

void	increment_word_var(int *total_word_count, int *in_word, char *code)
{
	if (ft_strncmp(code, "single", 7) == 0)
	{
		(*total_word_count)++;
		*in_word = 0;
	}
	else if (ft_strncmp(code, "pipe", 5) == 0)
	{
		(*total_word_count)++;
		*in_word = 0;
	}
	else if (ft_strncmp(code, "regular", 8) == 0)
		(*total_word_count)++;
}

int	process_character_wc(char *read_line, int *i,
		char delimiter, t_wc_state *st)
{
	int	quote_result;

	quote_result = count_quoted_word(read_line, i, &st->total_word_count);
	if (quote_result == FAILURE)
		return (FAILURE);
	else if (quote_result == SUCCESS)
	{
		st->in_word = 0;
		return (SUCCESS);
	}
	if (is_single_redirection(read_line[*i]))
	{
		increment_word_var(&st->total_word_count, &st->in_word, "single");
		if (read_line[*i + 1] && read_line[*i + 1] == read_line[*i])
			(*i)++;
	}
	else if (is_pipe(read_line[*i]))
		increment_word_var(&st->total_word_count, &st->in_word, "pipe");
	else if (process_regular_word(read_line, *i,
			delimiter, &st->in_word) == SUCCESS)
		increment_word_var(&st->total_word_count, &st->in_word, "regular");
	return (SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:24:17 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 19:24:17 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	count_rl_contents(char *read_line, char delimiter)
{
	int			i;
	t_wc_state	st;

	i = 0;
	st.total_word_count = 0;
	st.in_word = 0;
	while (read_line[i])
	{
		if (process_character_wc(read_line, &i, delimiter, &st) == FAILURE)
			return (FAILURE);
		i++;
	}
	return (st.total_word_count);
}

int	find_word_end(char *read_line, int start, char delimiter)
{
	int	end;
	int	quote_end;

	end = start;
	if (is_single_redirection(read_line[end]))
		return (get_redirection_length(read_line, end) + end);
	else if (is_pipe(read_line[end]))
		return (end + 1);
	while (read_line[end] && read_line[end] != delimiter)
	{
		if (is_single_quote(read_line[end])
			|| is_double_quote(read_line[end]))
		{
			quote_end = handle_quoted_word(read_line, end, 'S');
			if (quote_end == QUOTELESS)
				return (FAILURE);
			end = quote_end + 1;
		}
		else if (is_single_redirection(read_line[end])
			|| is_pipe(read_line[end]))
			return (end);
		else
			end++;
	}
	return (end);
}

char	*get_characters(char *read_line, int *current_i, char delimiter)
{
	char	*word;
	int		start;
	int		end;
	int		total_c;
	int		i;

	start = *current_i;
	end = find_word_end(read_line, start, delimiter);
	if (end == FAILURE)
		return (NULL);
	total_c = end - start;
	word = malloc((total_c + 1) * sizeof(char));
	if (!word)
		return (NULL);
	ft_memset(word, 0, total_c + 1);
	i = 0;
	while (i < total_c)
	{
		word[i] = read_line[start + i];
		i++;
	}
	*current_i = end;
	return (word);
}

char	**allocate_result_array(char *read_line, char delimiter)
{
	int		total_words;
	char	**result;

	total_words = count_rl_contents(read_line, delimiter);
	if (total_words == FAILURE)
		return (NULL);
	result = malloc((total_words + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	return (result);
}

int	get_words(char *read_line, char delimiter, char **result)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (read_line[i])
	{
		if (read_line[i] == delimiter)
			i++;
		else
		{
			result[k] = get_characters(read_line, &i, delimiter);
			if (!result[k])
			{
				result[k] = NULL;
				cleanup_partial_argv(result, k);
				return (FAILURE);
			}
			k++;
		}
	}
	result[k] = NULL;
	return (SUCCESS);
}

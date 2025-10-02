/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:24:52 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 19:24:52 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	**split_argv_contents(char *read_line, char delimiter)
{
	char	**result;

	result = allocate_result_array(read_line, delimiter);
	if (!result)
		return (NULL);
	if (get_words(read_line, delimiter, result) == FAILURE)
	{
		if (result)
			free(result);
		return (NULL);
	}
	return (result);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_args_length.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:46:40 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 17:46:40 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_new_args_length(char *string, t_env **env, int hd_delim)
{
	int		total_len;
	int		i;
	char	c;

	total_len = 0;
	i = 0;
	while (string[i])
	{
		c = string[i];
		if (is_single_quote(c))
			process_single_quote(string, &i, &total_len);
		else if (is_double_quote(c))
			total_len += process_double_quote(string, &i, env, hd_delim);
		else if (c == '$' && hd_delim != 1)
			process_variable(string, &i, &total_len, env);
		else if (c == ' ')
			i++;
		else
		{
			total_len++;
			i++;
		}
	}
	return (total_len);
}

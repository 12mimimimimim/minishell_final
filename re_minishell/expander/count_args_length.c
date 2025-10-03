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

int	process_positional_param(char *string, int *i, t_env **env)
{
	char	*expanded;
	int		total_len;
	int		j;

	total_len = 0;
	expanded = expand_value(&string[*i], env);
	if (expanded)
	{
		j = 0;
		while (expanded[j])
		{
			total_len++;
			j++;
		}
		free(expanded);
	}
	(*i) += 2;
	return (total_len);
}

static int	handle_special_chars(char *string, int *i, t_env **env, int flag)
{
	int		len;
	char	c;

	len = -1;
	c = string[*i];
	if (!string[*i])
		return (FAILURE);
	if (is_single_quote(c) && flag != 2)
		len = (process_single_quote(string, i, &len));
	else if (is_double_quote(c) && flag != 2)
		len = process_double_quote(string, i, env, flag);
	else if (c == '$' && flag != 1 && string[*i + 1]
		&& ((ft_isalpha(string[*i + 1]) || string[*i + 1] == '_'
				|| string[*i + 1] == '?' )))
		len = process_variable(string, i, env);
	else if (c == '$' && flag != 1 && string[*i + 1]
		&& ft_isdigit(string[*i + 1]))
		len = process_positional_param(string, i, env);
	return (len);
}

int	count_new_args_length(char *string, t_env **env, int flag)
{
	int		total_len;
	int		i;
	int		special_len;

	total_len = 0;
	i = 0;
	while (string[i])
	{
		special_len = handle_special_chars(string, &i, env, flag);
		if (special_len >= 0)
			total_len += special_len;
		else if (string[i] && string[i] == ' ' && flag != 2)
			i++;
		else if (string[i])
		{
			total_len++;
			i++;
		}
	}
	return (total_len);
}

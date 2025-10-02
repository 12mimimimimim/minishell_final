/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_args_length_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:46:05 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 17:46:05 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <string.h>

int	process_single_quote(char *string, int *i, int *total_len)
{
	int	trimmed_q_len;

	trimmed_q_len = count_trimmed_quotes(&string[*i], '\'');
	if (trimmed_q_len != QUOTELESS)
	{
		*total_len += trimmed_q_len;
		*i += trimmed_q_len + 2;
		return (SUCCESS);
	}
	return (FAILURE);
}

int	process_double_quote(char *string, int *i, t_env **env, int flag)
{
	int	total_len;

	(*i)++;
	total_len = 0;
	while (string[*i] && !is_double_quote(string[*i]))
	{
		if (string[*i] == '$' && flag != 1)
		{
			total_len += get_expanded_length(string, i, env);
			if (string[*i] == '?')
				(*i)++;
			else
				while (string[*i] && is_valid_var_char(string[*i]))
					(*i)++;
		}
		else
		{
			(*i)++;
			total_len++;
		}
	}
	if (string[*i] == '"')
		(*i)++;
	return (total_len);
}

int	process_variable(char *string, int *i, t_env **env)
{
	char	*expanded;
	int		total_len;

	total_len = 0;
	expanded = expand_value(&string[*i], env);
	if (expanded)
	{
		total_len += ft_strlen(expanded);
		free(expanded);
	}
	(*i)++;
	if (string[*i] == '?')
		(*i)++;
	else
		while (string[*i] && is_valid_var_char(string[*i]))
			(*i)++;
	return (total_len);
}

int	count_trimmed_quotes(char *string, char c)
{
	int	i;
	int	total_count;

	i = 1;
	total_count = 0;
	while (string[i] && string[i] != c)
	{
		total_count++;
		i++;
	}
	if (string[i] == c)
		return (total_count);
	return (QUOTELESS);
}

int	get_expanded_length(char *string, int *i, t_env **env)
{
	char	*expanded;
	int		len;

	(*i)++;
	if (string[*i] == '?')
	{
		expanded = expand_value(&string[*i - 1], env);
		len = ft_strlen(expanded);
		free(expanded);
		(*i)++;
		return (len);
	}
	if (is_valid_var_char(string[*i]))
	{
		expanded = expand_value(&string[*i - 1], env);
		len = ft_strlen(expanded);
		free(expanded);
		while (string[*i] && is_valid_var_char(string[*i]))
			(*i)++;
		return (len);
	}
	return (1);
}

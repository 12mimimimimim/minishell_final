/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_args_length_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:40:06 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 17:40:06 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	copy_single_quoted(char *string, int *i, t_exp_var *more)
{
	int	copied;

	copied = 0;
	(*i)++;
	while (string[*i] && !is_single_quote(string[*i]))
	{
		more->result[copied] = string[*i];
		copied++;
		(*i)++;
	}
	if (string[*i] == '\'')
		(*i)++;
	return (copied);
}

void	skip_variable_name(char *string, int *i)
{
	(*i)++;
	if (string[*i] == '?')
		(*i)++;
	else
	{
		while (string[*i] && is_valid_var_char(string[*i]))
			(*i)++;
	}
}

int	handle_dollar_expansion(char *string, int *i, char *result, t_env **env)
{
	char	*expanded;
	int		exp_len;
	int		copied;

	copied = 0;
	if (string[*i + 1] && is_double_quote(string[*i + 1]))
	{
		result[copied] = string[*i];
		copied++;
		(*i)++;
		return (copied);
	}
	expanded = expand_value(&string[*i], env);
	if (expanded)
	{
		exp_len = ft_strlen(expanded);
		ft_strcpy(result + copied, expanded);
		copied += exp_len;
		free(expanded);
	}
	skip_variable_name(string, i);
	return (copied);
}

int	copy_double_quoted(char *string, int *i, t_exp_var *more, t_env **env)
{
	int	copied;

	copied = 0;
	(*i)++;
	while (string[*i] && !is_double_quote(string[*i]))
	{
		if (string[*i] == '$' && !more->flag)
		{
			copied += handle_dollar_expansion(string, i,
					more->result + copied, env);
			if (string[*i] && is_double_quote(string[*i]))
				break ;
		}
		else
		{
			more->result[copied] = string[*i];
			copied++;
			(*i)++;
		}
	}
	if (string[*i] && is_double_quote(string[*i]))
		(*i)++;
	return (copied);
}

int	copy_variable(char *string, int *i, t_exp_var *more, t_env **env)
{
	char	*expanded;
	int		copied;

	expanded = expand_value(&string[*i], env);
	copied = 0;
	if (expanded)
	{
		copied = ft_strlen(expanded);
		ft_strcpy(more->result, expanded);
		free(expanded);
	}
	(*i)++;
	if (string[*i] == '?')
		(*i)++;
	else
	{
		while (string[*i] && is_valid_var_char(string[*i]))
			(*i)++;
	}
	return (copied);
}

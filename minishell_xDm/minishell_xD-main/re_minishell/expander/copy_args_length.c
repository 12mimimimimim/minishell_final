/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_args_length.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:40:08 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 17:40:08 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	copy_positional_param(char *string, int *i, t_exp_var *more, t_env **env)
{
	char	*expanded;
	int		copied;
	int		j;

	copied = 0;
	expanded = expand_value(&string[*i], env);
	if (expanded)
	{
		j = 0;
		while (expanded[j])
		{
			more->result[copied] = expanded[j];
			copied++;
			j++;
		}
		free(expanded);
	}
	(*i) += 2;
	return (copied);
}

static int	handle_quotes_and_variables(char *string, int *i, t_exp_var *more,
			t_env **env)
{
	int	copied;

	copied = 0;
	if (is_single_quote(string[*i]) && more->flag != 2)
	{
		more->had_quotes = 1;
		copied = copy_single_quoted(string, i, more);
	}
	else if (is_double_quote(string[*i]) && more->flag != 2)
	{
		more->had_quotes = 1;
		copied = copy_double_quoted(string, i, more, env);
	}
	else if (string[*i] == '$' && (ft_isalpha(string[*i + 1])
			|| string[*i + 1] == '_' || string[*i + 1] == '?'
			|| string[*i + 1] == '$'))
	{
		if (more->flag == 0 || more->flag == 2)
			copied = copy_variable(string, i, more, env);
	}
	else if (string[*i] == '$' && more->flag != 1 && ft_isdigit(string[*i + 1]))
		copied = copy_positional_param(string, i, more, env);
	return (copied);
}

int	process_character(char *string, int *i, t_exp_var *more, t_env **env)
{
	int	copied;

	copied = handle_quotes_and_variables(string, i, more, env);
	if (copied > 0)
		return (copied);
	if (string[*i] && string[*i] == ' ' && more->flag != 2)
		(*i)++;
	if (string[*i] && !is_double_quote(string[*i])
		&& !is_single_quote(string[*i]) && more->flag != 2 && string[*i] != '$')
	{
		more->result[0] = string[*i];
		(*i)++;
		copied = 1;
	}
	if (string[*i] && more->flag == 2)
	{
		more->result[0] = string[*i];
		(*i)++;
		copied = 1;
	}
	return (copied);
}

char	*process_mixed_args(char *string, t_env **env, int flag,
							int *had_quotes)
{
	int			i;
	int			total_c;
	char		*result;
	t_exp_var	more;

	i = 0;
	if (!string)
		return (NULL);
	total_c = count_new_args_length(string, env, flag);
	result = ft_calloc((total_c + 1), sizeof(char));
	if (!result)
		return (NULL);
	more.result = result;
	more.flag = flag;
	more.had_quotes = 0;
	if (had_quotes && *had_quotes)
		more.had_quotes = *had_quotes;
	while (string[i])
		more.result += process_character(string, &i, &more, env);
	*more.result = '\0';
	if (had_quotes)
		*had_quotes = more.had_quotes;
	return (result);
}

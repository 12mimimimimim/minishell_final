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

int	process_character(char *string, int *i, t_exp_var *more, t_env **env)
{
	int	copied;

	copied = 0;
	if (is_single_quote(string[*i]))
		copied = copy_single_quoted(string, i, more);
	else if (is_double_quote(string[*i]))
		copied = copy_double_quoted(string, i, more, env);
	else if (string[*i] == '$' && (more->flag == 0 || more->flag == 2))
		copied = copy_variable(string, i, more, env);
	else if (string[*i] == ' ' && more->flag != 2)
		(*i)++;
	else
	{
		more->result[0] = string[*i];
		(*i)++;
		copied = 1;
	}
	return (copied);
}

char	*process_mixed_args(char *string, t_env **env, int flag)
{
	int			i;
	int			total_c;
	char		*result;
	t_exp_var	more;

	i = 0;
	if (flag == 1)
		total_c = count_new_args_length(string, env, 1);
	else
		total_c = count_new_args_length(string, env, 0);
	result = ft_calloc((total_c + 1), sizeof(char));
	if (!result)
		return (NULL);
	more.result = result;
	more.flag = flag;
	while (string[i])
		more.result += process_character(string, &i, &more, env);
	*more.result = '\0';
	return (result);
}

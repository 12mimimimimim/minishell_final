/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:43:16 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 18:43:16 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_value(char *string, t_env **env)
{
	char	*key;
	char	*val;

	if (string[0] != '$')
		return (ft_strdup(string));
	if (string[0] == '$' && (!string[1] || string[1] == ' '))
		return (ft_strdup("$"));
	if (string[1] == '?')
		return (ft_itoa(last_exit_status(0, 0)));
	if (string[1] == '\0'
		|| (!is_valid_var_char(string[1]) && string[1] != '?'))
		return (ft_strdup(""));
	key = extract_key(string + 1);
	if (!key)
		return (ft_strdup(""));
	val = get_env_value(key, env);
	free(key);
	if (val)
		return (ft_strdup(val));
	return (ft_strdup(""));
}

int	count_args(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	return (i);
}

char	**expanded_argv(char **argv, t_env **env)
{
	int		i;
	int		total;
	char	**new_argv;

	i = 0;
	total = count_args(argv);
	new_argv = malloc((total + 1) * sizeof(char *));
	if (!new_argv)
		return (NULL);
	while (argv[i])
	{
		new_argv[i] = process_mixed_args(argv[i], env, 0);
		if (!new_argv[i])
		{
			cleanup_partial_argv(new_argv, i);
			return (NULL);
		}
		i++;
	}
	new_argv[i] = NULL;
	return (new_argv);
}

int	expand_command_node(t_ast *node, t_env **env)
{
	char	**old_argv;

	if (node->type == AST_COMMAND && node->argv)
	{
		old_argv = node->argv;
		node->argv = expanded_argv(node->argv, env);
		if (!node->argv)
		{
			node->argv = old_argv;
			return (FAILURE);
		}
		if (old_argv)
			free_argv(old_argv);
	}
	return (SUCCESS);
}

int	expand_redirection_node(t_ast *node, t_env **env)
{
	char	*old;

	if ((node->type == AST_REDIR_IN || node->type == AST_REDIR_OUT
			|| node->type == AST_REDIR_APPEND) && node->filename)
	{
		old = node->filename;
		node->filename = process_mixed_args(node->filename, env, 0);
		if (!node->filename)
		{
			node->filename = old;
			return (FAILURE);
		}
		free(old);
	}
	return (SUCCESS);
}

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
	if (string[0] == '$' && (!string[1] || string[1] == ' '
			|| string[1] == '$'))
		return (ft_strdup("$"));
	if (string[1] == '?')
		return (ft_itoa(last_exit_status(0, 0)));
	if (string[1] == '\0' || (!is_valid_var_char(string[1])
			&& string[1] != '?'))
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

char	**expanded_argv(char **argv, t_env **env)
{
	int		i;
	int		j;
	int		total;
	int		had_quotes;
	char	**new_argv;

	i = -1;
	j = 0;
	had_quotes = 0;
	total = count_args(argv);
	new_argv = malloc((total + 1) * sizeof(char *));
	if (!new_argv)
		return (NULL);
	while (argv[++i])
	{
		new_argv[j] = process_mixed_args(argv[i], env, 0, &had_quotes);
		if (!new_argv[j])
			return (cleanup_partial_argv(new_argv, j), NULL);
		if (new_argv[j][0] == '\0' && !had_quotes)
			free(new_argv[j]);
		else
			j++;
	}
	new_argv[j] = NULL;
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

static int	file_error(char **file, char *old_file, char *code)
{
	if (ft_strncmp("empty_var", code, 10) == 0)
	{
		free(*file);
		*file = NULL;
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(old_file, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		free(old_file);
		return (FAILURE);
	}
	else if (ft_strncmp("empty_str", code, 10) == 0)
	{
		free(*file);
		*file = NULL;
		ft_putstr_fd("minishell: No such file or directory\n", 2);
		free(old_file);
		return (FAILURE);
	}
	return (FAILURE);
}

int	expand_redirection_node(t_ast *node, t_env **env)
{
	char	*old;
	int		has_quotes;

	has_quotes = 0;
	if ((node->type == AST_REDIR_IN || node->type == AST_REDIR_OUT
			|| node->type == AST_REDIR_APPEND) && node->filename)
	{
		old = node->filename;
		node->filename = process_mixed_args(
				node->filename, env, 0, &has_quotes);
		if (!node->filename)
			return (node->filename = old, FAILURE);
		if (node->filename[0] == '\0' && !has_quotes)
			return (file_error(&node->filename, old, "empty_var"));
		else if (node->filename[0] == '\0' && has_quotes)
			return (file_error(&node->filename, old, "empty_str"));
		free(old);
	}
	return (SUCCESS);
}

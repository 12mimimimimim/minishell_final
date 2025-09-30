/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:41:59 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 18:41:59 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_mixed_args(char *string, t_env **env, int flag);

char	*extract_key(const char *str)
{
	int		len;
	char	*key;

	len = 0;
	while (str[len] && is_valid_var_char(str[len]))
		len++;
	key = ft_strndup(str, len);
	return (key);
}

t_ast	*expand_node(t_ast *node, t_env **env)
{
	if (!node)
		return (NULL);
	if (!expand_command_node(node, env))
		return (NULL);
	if (!expand_redirection_node(node, env))
		return (NULL);
	return (node);
}

t_ast	*expander(t_ast *node, t_env **env)
{
	if (!node)
		return (NULL);
	if (!expand_node(node, env))
		return (NULL);
	if (node->left && !expander(node->left, env))
		return (NULL);
	if (node->right && !expander(node->right, env))
		return (NULL);
	return (node);
}

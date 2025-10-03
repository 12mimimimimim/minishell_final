/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:40:54 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/18 18:40:54 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*process_heredoc_line(char *line, int expand, t_env **env)
{
	if (!expand)
		return (ft_strdup(line));
	return (process_mixed_args(line, env, 2, NULL));
}

int	is_quoted(const char *s)
{
	int	len;

	len = ft_strlen(s);
	if (len >= 2 && ((s[0] == '\'' && s[len - 1] == '\'')
			|| (s[0] == '"' && s[len - 1] == '"')))
		return (s[0]);
	return (0);
}

int	is_valid_var_char(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9') || c == '_');
}

char	*get_env_value(const char *key, t_env **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		if (ft_strcmp(env[i]->key, key) == 0)
			return (env[i]->value);
		i++;
	}
	return (NULL);
}

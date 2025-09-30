/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: didris <didris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:02:56 by didris            #+#    #+#             */
/*   Updated: 2025/09/15 14:02:56 by didris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	find_env_index(t_env **env, char *key)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		if (ft_strncmp(env[i]->key, key, ft_strlen(key) + 1) == 0)
			return (i);
		i++;
	}
	return (-1);
}

t_env	*create_env_node(char *env_str)
{
	t_env	*node;
	int		eq_i;

	eq_i = find_eq(env_str);
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	if (eq_i == -1)
	{
		node->key = ft_strdup(env_str);
		node->value = NULL;
	}
	else
	{
		node->key = ft_strndup(env_str, eq_i);
		node->value = ft_strdup(env_str + eq_i + 1);
	}
	node->exported = 1;
	if (!node->key || (eq_i != -1 && !node->value))
	{
		free(node->key);
		free(node->value);
		return (free(node), NULL);
	}
	return (node);
}

t_env	**init_env(char **envp)
{
	int		i;
	int		j;
	t_env	**env_list;

	i = 0;
	j = 0;
	env_list = malloc(sizeof(t_env *) * (count_envp(envp) + 1));
	if (!env_list)
		return (NULL);
	while (envp[i])
	{
		env_list[j] = create_env_node(envp[i]);
		if (!env_list[j])
			return (free_env(env_list), NULL);
		j++;
		i++;
	}
	env_list[j] = NULL;
	return (env_list);
}

char	**env_to_char(t_env **env)
{
	int		i;
	int		j;
	char	**envp;

	i = 0;
	j = 0;
	envp = malloc(sizeof(char *) * (count_env_list(env) + 1));
	if (!envp)
		return (NULL);
	while (env[i])
	{
		if (env[i]->exported)
		{
			if (!format_env_node(envp, env[i], j))
				return (free_argv(envp), NULL);
			j++;
		}
		i++;
	}
	envp[j] = NULL;
	return (envp);
}

t_env	**copy_env(t_env ***env, int count)
{
	t_env	**new_env;
	int		i;

	i = 0;
	new_env = malloc(sizeof(t_env *) * (count + 2));
	if (!new_env)
		return (NULL);
	while (i < count)
	{
		new_env[i] = copy_env_var((*env)[i]);
		if (!new_env[i])
		{
			new_env[count] = NULL;
			free_env(new_env);
			return (NULL);
		}
		i++;
	}
	return (new_env);
}

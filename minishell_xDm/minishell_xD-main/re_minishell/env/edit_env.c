/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: didris <didris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:05:36 by didris            #+#    #+#             */
/*   Updated: 2025/09/15 14:05:36 by didris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	update_env(t_env **env, char *key, char *value)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i]->key, key, ft_strlen(key) + 1) == 0)
		{
			if (env[i]->value)
				free(env[i]->value);
			if (value)
				env[i]->value = ft_strdup(value);
			else
				env[i]->value = NULL;
			env[i]->exported = 1;
			return (1);
		}
		i++;
	}
	return (0);
}

int	add_to_env(t_env ***env, char *env_str, int exported)
{
	t_env	**new_env;
	t_env	*new_var;
	int		count;

	count = 0;
	while ((*env) && (*env)[count])
		count++;
	new_env = copy_env(env, count);
	if (!new_env)
		return (-1);
	new_var = create_env_node(env_str);
	if (!new_var)
	{
		free(new_env);
		return (-1);
	}
	new_var->exported = exported;
	new_env[count] = new_var;
	new_env[count + 1] = NULL;
	free_env(*env);
	*env = new_env;
	return (0);
}

int	remove_env(t_env **env, char *key)
{
	int	i;

	i = find_env_index(env, key);
	if (i == -1)
		return (0);
	free(env[i]->key);
	free(env[i]->value);
	free(env[i]);
	while (env[i])
	{
		env[i] = env[i + 1];
		i++;
	}
	return (1);
}

void	sort_env(t_env **env)
{
	int		i;
	int		j;
	t_env	*temp;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strncmp(env[i]->key, env[j]->key,
					ft_strlen(env[i]->key) + 1) > 0)
			{
				temp = env[i];
				env[i] = env[j];
				env[j] = temp;
			}
			j++;
		}
		i++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: didris <didris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 21:17:30 by didris            #+#    #+#             */
/*   Updated: 2025/07/10 21:17:30 by didris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

t_env	*copy_env_var(t_env *src)
{
	t_env	*new_entry;

	new_entry = malloc(sizeof(t_env));
	if (!new_entry)
		return (NULL);
	new_entry->key = ft_strdup(src->key);
	if (!new_entry->key)
	{
		free(new_entry);
		return (NULL);
	}
	new_entry->value = NULL;
	if (src->value)
	{
		new_entry->value = ft_strdup(src->value);
		if (!new_entry->value)
		{
			free(new_entry->key);
			free(new_entry);
			return (NULL);
		}
	}
	new_entry->exported = src->exported;
	return (new_entry);
}

int	format_env_node(char **envp, t_env *node, int j)
{
	int	len;

	len = ft_strlen(node->key) + 2;
	if (node->value)
		len += ft_strlen(node->value);
	envp[j] = malloc(len * sizeof(char));
	if (!envp[j])
		return (FAILURE);
	ft_strlcpy(envp[j], node->key, len);
	ft_strlcat(envp[j], "=", len);
	if (node->value)
		ft_strlcat(envp[j], node->value, len);
	return (SUCCESS);
}

void	free_env(t_env **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		free(env[i]->key);
		free(env[i]->value);
		free(env[i]);
		i++;
	}
	free(env);
}

int	count_env_list(t_env **env)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (env[i])
	{
		if (env[i]->exported)
			count++;
		i++;
	}
	return (count);
}

int	count_envp(char *envp[])
{
	int	count;

	count = 0;
	while (envp[count])
	{
		count++;
	}
	return (count);
}

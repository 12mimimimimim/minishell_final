/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:50:02 by didris            #+#    #+#             */
/*   Updated: 2025/09/29 19:32:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	find_eq(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if (str[i] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	export_with_eq(t_env ***env, char *argv)
{
	int		eq_index;
	char	*key;
	char	*value;

	eq_index = find_eq(argv);
	if (eq_index == -1)
		return (1);
	key = ft_strndup(argv, eq_index);
	value = ft_strdup(argv + eq_index + 1);
	if (!key || !value)
	{
		free(key);
		free(value);
		return (1);
	}
	if (find_env_index(*env, key) != -1)
		update_env(*env, key, value);
	else
	{
		if (add_to_env(env, argv, 1) == -1)
			return (free(key), free(value), 1);
	}
	return (free(key), free(value), 0);
}

int	export_without_eq(t_env ***env, char *argv)
{
	int	env_i;

	env_i = find_env_index(*env, argv);
	if (env_i == -1)
	{
		if (add_to_env(env, argv, 1) == -1)
			return (1);
	}
	else
		(*env)[env_i]->exported = 1;
	return (0);
}

int	export_loop(t_env ***env, char *argv)
{
	int	eq_index;

	eq_index = find_eq(argv);
	if (!is_valid_key(argv))
	{
		write(2, "minishell: export: `", 20);
		write(2, argv, ft_strlen(argv));
		write(2, "': not a valid identifier\n", 26);
		return (1);
	}
	if (eq_index == -1)
		export_without_eq(env, argv);
	else
		export_with_eq(env, argv);
	return (0);
}

void	print_for_export(t_env **env)
{
	int		i;
	int		count;
	t_env	**sorted_env;

	i = 0;
	count = count_env_list(env);
	sorted_env = malloc(sizeof(t_env *) * (count + 1));
	if (!sorted_env)
	{
		perror("minishell: export");
		return ;
	}
	while (i < count)
	{
		sorted_env[i] = env[i];
		i++;
	}
	sorted_env[i] = NULL;
	sort_env(sorted_env);
	print_sorted_env(sorted_env);
	free(sorted_env);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:55:53 by didris            #+#    #+#             */
/*   Updated: 2025/10/01 18:55:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	is_n_flag(char *str)
{
	int	i;

	i = 1;
	if (!str || str[0] != '-')
		return (0);
	if (str[1] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	is_valid_key(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	print_sorted_env(t_env **sorted)
{
	int	i;

	i = 0;
	while (sorted[i])
	{
		if (sorted[i]->value)
		{
			write(1, "declare -x ", 11);
			write(1, sorted[i]->key, ft_strlen(sorted[i]->key));
			write(1, "=\"", 2);
			write(1, sorted[i]->value, ft_strlen(sorted[i]->value));
			write(1, "\"\n", 2);
		}
		else
		{
			write(1, "declare -x ", 11);
			write(1, sorted[i]->key, ft_strlen(sorted[i]->key));
			write(1, "\n", 1);
		}
		i++;
	}
}

int	is_duplicate_unset(char **argv, int curr)
{
	int	i;

	i = 1;
	while (i < curr)
	{
		if (ft_strncmp(argv[curr], argv[i], ft_strlen(argv[curr])) == 0)
			return (1);
		i++;
	}
	return (0);
}

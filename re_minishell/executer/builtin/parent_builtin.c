/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:10:17 by didris            #+#    #+#             */
/*   Updated: 2025/09/29 19:33:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	builtin_cd(char **argv)
{
	if (!argv[1])
	{
		write(2, "minishell: cd: missing argument\n", 32);
		return (1);
	}
	if (argv[2])
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		return (1);
	}
	if (argv[1][0] == '~' || argv[1][0] == '-' || argv[1][0] == '$')
	{
		write(2, "minishell: cd: unsupported path syntax\n", 39);
		return (1);
	}
	if (chdir(argv[1]) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	builtin_exit(char **argv, t_clean *clean)
{
	int	exit_code;

	if (!argv[1])
	{
		write(1, "exit\n", 5);
		clean_child_exit(clean, 0);
	}
	if (!is_numeric(argv[1]))
	{
		write(2, "minishell: exit: ", 17);
		write(2, argv[1], ft_strlen(argv[1]));
		write(2, ": numeric argument required\n", 27);
		write(1, "exit\n", 5);
		clean_child_exit(clean, 2);
	}
	if (argv[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return (1);
	}
	exit_code = ft_atoi(argv[1]) % 256;
	clean_child_exit(clean, exit_code);
	return (exit_code);
}

int	builtin_unset(t_env ***env, char **argv)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (argv[i])
	{
		if (is_duplicate_unset(argv, i))
		{
			i++;
			continue ;
		}
		if (!is_valid_key(argv[i]))
		{
			write(2, "minishell: unset: `", 19);
			write(2, argv[i], ft_strlen(argv[i]));
			write(2, "': not a valid identifier\n", 26);
			status = 1;
			i++;
			continue ;
		}
		remove_env(*env, argv[i]);
		i++;
	}
	return (status);
}

int	builtin_export(t_env ***env, char **argv)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!argv[1])
	{
		print_for_export(*env);
		return (0);
	}
	while (argv[i])
	{
		if (export_loop(env, argv[i]) != 0)
			status = 1;
		i++;
	}
	return (status);
}

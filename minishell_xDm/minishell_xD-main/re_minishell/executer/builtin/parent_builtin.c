/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:10:17 by didris            #+#    #+#             */
/*   Updated: 2025/10/01 18:56:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	builtin_cd(char **argv, t_env **env)
{
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];
	int		pwd_i;

	if (!argv[1])
		return (ft_putendl_fd("minishell: cd: missing argument", 2), 1);
	if (argv[2])
		return (ft_putendl_fd("minishell: cd: too many arguments", 2), 1);
	if (argv[1][0] == '~' || argv[1][0] == '-' || argv[1][0] == '$')
		return (ft_putendl_fd("minishell: cd: unsupported path syntax", 2), 1);
	if (!getcwd(old_pwd, sizeof(old_pwd)))
	{
		pwd_i = find_env_index(env, "PWD");
		if (pwd_i >= 0 && env[pwd_i]->value)
			ft_strlcpy(old_pwd, env[pwd_i]->value, sizeof(old_pwd));
	}
	if (chdir(argv[1]) != 0)
		return (perror("cd"), 1);
	if (getcwd(new_pwd, sizeof(new_pwd)))
		update_env(env, "PWD", new_pwd);
	update_env(env, "OLDPWD", old_pwd);
	return (0);
}

int	builtin_exit(char **argv, t_clean *clean)
{
	long long	exit_code;

	if (!argv[1])
	{
		write(1, "exit\n", 5);
		clean_child_exit(clean, 0);
	}
	if (!is_valid_exit_arg(argv[1], &exit_code))
	{
		write(2, "minishell: exit: ", 17);
		write(2, argv[1], ft_strlen(argv[1]));
		write(2, ": numeric argument required\n", 28);
		write(1, "exit\n", 5);
		clean_child_exit(clean, 2);
	}
	if (argv[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return (1);
	}
	write(1, "exit\n", 5);
	clean_child_exit(clean, (unsigned char)exit_code);
	return ((unsigned char)exit_code);
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

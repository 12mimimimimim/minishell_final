/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 11:25:56 by didris            #+#    #+#             */
/*   Updated: 2025/09/24 14:50:21 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	builtin_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("pwd");
		return (1);
	}
	write(1, pwd, ft_strlen(pwd));
	write(1, "\n", 1);
	free(pwd);
	return (0);
}

int	builtin_echo(char **argv)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (argv[i] && is_n_flag(argv[i]))
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		write(1, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}

int	builtin_env(t_env **env, char **argv)
{
	int	i;

	i = 0;
	if (!env)
		return (0);
	if (argv && argv[1])
	{
		write(2, "minishell: env: ", 16);
		write(2, argv[i], ft_strlen(argv[i]));
		write(2, ": No such file or directory\n", 28);
		return (1);
	}
	while (env && env[i])
	{
		if (env[i]->exported && env[i]->value)
		{
			write(1, env[i]->key, ft_strlen(env[i]->key));
			write(1, "=", 1);
			write(1, env[i]->value, ft_strlen(env[i]->value));
			write(1, "\n", 1);
		}
		i++;
	}
	return (0);
}

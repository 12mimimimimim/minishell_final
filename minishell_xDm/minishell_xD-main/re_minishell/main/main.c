/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 20:25:37 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/29 21:40:00 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	shlvl_limit_warning(int *shlvl)
{
	char	*str;

	str = ft_itoa(*shlvl);
	if (!str)
		return ;
	write(2, "minishell: warning: shell level (", 34);
	write(2, str, ft_strlen(str));
	write(2, ") too high, resetting to 1\n", 27);
	free(str);
	*shlvl = 1;
}

void	increment_shlvl(t_env **env)
{
	int		i;
	int		shlvl;
	char	*new_shlvl;

	i = find_env_index(env, "SHLVL");
	shlvl = 0;
	if (i >= 0 && env[i]->value)
		shlvl = ft_atoi(env[i]->value);
	shlvl++;
	if (shlvl >= 1000)
		shlvl_limit_warning(&shlvl);
	new_shlvl = ft_itoa(shlvl);
	if (!new_shlvl)
		return ;
	if (i >= 0)
	{
		free(env[i]->value);
		env[i]->value = new_shlvl;
	}
	else
	{
		update_env(env, "SHLVL", new_shlvl);
		free(new_shlvl);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_env	**env_copy;
	int		exit_code;

	(void)argc;
	(void)argv;
	exit_code = 0;
	env_copy = init_env(envp);
	if (!env_copy)
		return (1);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	increment_shlvl(env_copy);
	shell_loop(&env_copy, &exit_code);
	if (env_copy)
		free_env(env_copy);
	return (exit_code);
}

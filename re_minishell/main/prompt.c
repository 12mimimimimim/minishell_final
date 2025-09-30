/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 20:28:28 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/29 21:50:00 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*is_interactive_mode(char *line)
{
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
	{
		line = readline("minishell$ ");
		if (line && *line)
			add_history(line);
	}
	return (line);
}

char	*non_interactive_mode(char *line, t_env **env_copy, int *exit_code)
{
	char	*newline;

	(void)env_copy;
	line = get_next_line(STDIN_FILENO);
	if (!line)
		exit(*exit_code);
	newline = ft_strchr(line, '\n');
	if (newline)
		*newline = '\0';
	return (line);
}

int	is_builtin(const char *value)
{
	return (ft_strncmp(value, "echo", 5) == 0
		|| ft_strncmp(value, "cd", 3) == 0
		|| ft_strncmp(value, "pwd", 4) == 0
		|| ft_strncmp(value, "export", 7) == 0
		|| ft_strncmp(value, "unset", 6) == 0
		|| ft_strncmp(value, "env", 4) == 0
		|| ft_strncmp(value, "exit", 5) == 0);
}

void	clean_exit(int exit_code, t_env **env_copy)
{
	if (env_copy && *env_copy)
		free_env(env_copy);
	rl_clear_history();
	exit(exit_code);
}

char	*prompt(int exit_code, t_env **env_copy)
{
	char	*line;

	line = NULL;
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
	{
		line = is_interactive_mode(line);
		if (!line)
		{
			ft_putstr_fd("exit\n", 1);
			close_fds();
			clean_exit(exit_code, env_copy);
		}
		if (line[0] == '\0')
		{
			free(line);
			return (NULL);
		}
	}
	else
		line = non_interactive_mode(line, env_copy, &exit_code);
	return (line);
}

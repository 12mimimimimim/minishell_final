/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_child.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:52:47 by didris            #+#    #+#             */
/*   Updated: 2025/09/30 11:41:21 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static void	clean_cmd_child(char **env_c, t_clean *clean, int code)
{
	free_argv(env_c);
	close_fds();
	clean_child_exit(clean, code);
}

static void	cmd_error(char *cmd, char **env_c, t_clean *clean)
{
	write(2, "minishell: ", 11);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": command not found\n", 21);
	clean_cmd_child(env_c, clean, 127);
}

static void	execute_absolute_path(t_ast *node, char **env_c, t_clean *clean)
{
	int	check_result;

	check_result = is_executable_path(node->argv[0]);
	if (check_result != 0)
		clean_cmd_child(env_c, clean, check_result);
	execve(node->argv[0], node->argv, env_c);
	perror("execve");
	clean_cmd_child(env_c, clean, 126);
}

static void	empty_cmd_args(t_ast *node, char **env_c, t_clean *clean)
{
	int	i;

	if (!node->argv || !node->argv[0])
		clean_cmd_child(env_c, clean, 0);
	i = 0;
	free(node->argv[0]);
	while (node->argv[i + 1])
	{
		node->argv[i] = node->argv[i + 1];
		i++;
	}
	node->argv[i] = NULL;
}

void	child_process(t_ast *node, char **env_c, t_env **env, t_clean *clean)
{
	char	*cmd_path;
	int		code;

	setup_child_signals();
	while (node->argv && node->argv[0] && node->argv[0][0] == '\0')
		empty_cmd_args(node, env_c, clean);
	if (!node->argv || !node->argv[0])
		clean_cmd_child(env_c, clean, 0);
	if (is_builtin(node->argv[0]))
	{
		code = execute_builtin(node->argv, &env, clean);
		clean_cmd_child(env_c, clean, code);
	}
	if (ft_strchr(node->argv[0], '/'))
		execute_absolute_path(node, env_c, clean);
	cmd_path = validate_cmd(node->argv[0]);
	if (!cmd_path)
		cmd_error(node->argv[0], env_c, clean);
	execve(cmd_path, node->argv, env_c);
	perror("execve");
	free(cmd_path);
	clean_cmd_child(env_c, clean, 126);
}

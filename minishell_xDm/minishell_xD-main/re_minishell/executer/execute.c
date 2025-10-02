/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: didris <didris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 13:49:10 by didris            #+#    #+#             */
/*   Updated: 2025/07/05 13:49:10 by didris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static int	execute_command(t_ast *node, t_env **env, t_clean *clean)
{
	pid_t	pid;
	char	**env_c;

	if (!node || !node->argv || !node->argv[0])
		return (0);
	env_c = env_to_char(env);
	pid = fork();
	if (pid < 0)
	{
		free_argv(env_c);
		return (perror("fork"), 1);
	}
	if (pid == 0)
		child_process(node, env_c, env, clean);
	else
	{
		free_argv(env_c);
		return (parent_wait(pid));
	}
	return (0);
}

static int	execute_redir(t_ast *node, t_env **env, t_clean *clean)
{
	int		pid;
	t_ast	*cmd;
	int		code;

	cmd = find_cmd_node(node);
	if (validate_redirs(node) != 0)
		return (1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		setup_child_signals();
		code = apply_redir(node, env, clean);
		if (code != 0)
			clean_child_exit(clean, code);
		if (cmd)
			code = execute(cmd, &env, clean);
		else
			code = execute(NULL, &env, clean);
		clean_child_exit(clean, code);
	}
	return (parent_wait(pid));
}

int	execute_builtin(char **argv, t_env ***env, t_clean *clean)
{
	if (ft_strncmp(argv[0], "cd", 3) == 0)
		return (builtin_cd(argv, *env));
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		return (builtin_echo(argv));
	if (ft_strncmp(argv[0], "pwd", 4) == 0)
		return (builtin_pwd(*env));
	if (ft_strncmp(argv[0], "export", 7) == 0)
		return (builtin_export(env, argv));
	if (ft_strncmp(argv[0], "unset", 6) == 0)
		return (builtin_unset(env, argv));
	if (ft_strncmp(argv[0], "env", 4) == 0)
		return (builtin_env(*env, argv));
	if (ft_strncmp(argv[0], "exit", 5) == 0)
		return (builtin_exit(argv, clean));
	return (1);
}

static int	execute_pipe(t_ast *left, t_ast *right, t_env **env, t_clean *clean)
{
	int	pipe_fds[2];
	int	l_pid;
	int	r_pid;

	if (handle_pipe_hd(left, right, env, clean) < 0)
		return (130);
	if (pipe(pipe_fds) == -1)
		return (pipe_cleanup(right, left, NULL, "pipe"), 1);
	l_pid = fork();
	if (l_pid < 0)
		return (pipe_cleanup(right, left, NULL, "fork"), 1);
	if (l_pid == 0)
		(close_tree_heredocs(right), pipe_l_child(left, pipe_fds, env, clean));
	r_pid = fork();
	if (r_pid < 0)
	{
		waitpid(l_pid, NULL, 0);
		return (pipe_cleanup(right, left, pipe_fds, "fork"), 1);
	}
	if (r_pid == 0)
		(close_tree_heredocs(left), pipe_r_child(right, pipe_fds, env, clean));
	pipe_cleanup(right, left, NULL, NULL);
	return (pipe_parent(l_pid, r_pid, pipe_fds));
}

int	execute(t_ast *node, t_env ***env, t_clean *clean)
{
	int	exit_code;

	if (!node)
		return (1);
	if (node->type == AST_PIPE)
	{
		exit_code = execute_pipe(node->left, node->right, *env, clean);
		return (exit_code);
	}
	if (node->type == AST_COMMAND)
	{
		if (is_parent_command(node))
			exit_code = execute_builtin(node->argv, env, clean);
		else
			exit_code = execute_command(node, *env, clean);
		return (exit_code);
	}
	if (node->type >= AST_REDIR_IN && node->type <= AST_HEREDOC)
	{
		exit_code = execute_redir(node, *env, clean);
		return (exit_code);
	}
	return (1);
}

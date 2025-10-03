/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:19:59 by didris            #+#    #+#             */
/*   Updated: 2025/09/24 13:35:00 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	is_parent_command(t_ast *node)
{
	if (!node || !node->argv || !node->argv[0])
		return (0);
	if (ft_strncmp(node->argv[0], "cd", 3) == 0
		|| ft_strncmp(node->argv[0], "exit", 5) == 0
		|| ft_strncmp(node->argv[0], "export", 7) == 0
		|| ft_strncmp(node->argv[0], "unset", 6) == 0)
		return (1);
	return (0);
}

int	parent_wait(pid_t pid)
{
	int	status;
	int	code;

	status = 0;
	signal(SIGINT, SIG_IGN);
	if (waitpid(pid, &status, 0) == -1)
		perror("waitpid");
	if (WIFEXITED(status))
		code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		code = 128 + WTERMSIG(status);
	else
		code = 1;
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		write(1, "\n", 1);
	signal(SIGINT, signal_handler);
	return (code);
}

int	last_exit_status(int set, int value)
{
	static int	status;

	if (set)
		status = value;
	return (status);
}

void	clean_child_exit(t_clean *clean, int exit_code)
{
	if (clean)
	{
		free_argv(clean->split_argv);
		free_tokens(clean->tokens);
		free_ast(clean->main_node);
		free_env(clean->env_cpy);
	}
	close_fds();
	exit(exit_code);
}

t_ast	*find_cmd_node(t_ast *node)
{
	t_ast	*cmd;

	cmd = node;
	while (cmd && cmd->type >= AST_REDIR_IN && cmd->type <= AST_HEREDOC)
		cmd = cmd->left;
	return (cmd);
}

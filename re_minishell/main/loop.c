/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:20:28 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/29 22:10:00 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	executer(t_ast *ast, t_env ***env_copy, t_token *tokens,
					char **split_argv)
{
	int		exit_code;
	t_clean	clean;

	if (!ast)
		return ;
	clean.split_argv = split_argv;
	clean.main_node = ast;
	clean.tokens = tokens;
	clean.env_cpy = *env_copy;
	expander(ast, *env_copy);
	clean.main_node = ast;
	exit_code = execute(ast, env_copy, &clean);
	last_exit_status(1, exit_code);
}

t_token	*prepare_tokens(char *line, char ***split_argv)
{
	t_token	*tokens;

	*split_argv = NULL;
	if (check_for_bash_syntax_error(line) == FAILURE)
		return (NULL);
	else if (check_for_bash_syntax_error(line) == QUOTELESS)
		return (ft_putstr_fd("doesn't support unclosed quotes :|\n", 2), NULL);
	*split_argv = split_argv_contents(line, ' ');
	if (!*split_argv || !(*split_argv)[0])
	{
		if (*split_argv)
			free_argv(*split_argv);
		*split_argv = NULL;
		return (NULL);
	}
	tokens = tokenator(*split_argv);
	if (!tokens)
	{
		if (*split_argv)
			free_argv(*split_argv);
		*split_argv = NULL;
		return (NULL);
	}
	return (tokens);
}

t_ast	*parse_input_line(char *line, char ***split_argv, t_token **token_head)
{
	t_token	*tokens;
	t_ast	*parsed_ast;

	tokens = prepare_tokens(line, split_argv);
	*token_head = tokens;
	if (!tokens)
		return (NULL);
	parsed_ast = parser(&tokens);
	if (!parsed_ast)
	{
		cleanup_iteration(NULL, split_argv, token_head, NULL);
		return (NULL);
	}
	return (parsed_ast);
}

void	process_command(char *read_line, t_env ***env_copy, int *exit_code)
{
	char	**split_argv;
	t_token	*token_head;
	t_ast	*parsed_ast;

	split_argv = NULL;
	token_head = NULL;
	parsed_ast = parse_input_line(read_line, &split_argv, &token_head);
	if (!parsed_ast)
	{
		*exit_code = last_exit_status(1, 2);
		cleanup_iteration(NULL, &split_argv, &token_head, NULL);
		return ;
	}
	executer(parsed_ast, env_copy, token_head, split_argv);
	*exit_code = last_exit_status(0, 0);
	cleanup_iteration(NULL, &split_argv, &token_head, &parsed_ast);
}

void	shell_loop(t_env ***env_copy, int *exit_code)
{
	char	*read_line;

	while (1)
	{
		g_sigint_received = 0;
		read_line = prompt(*exit_code, *env_copy);
		if (!read_line)
			continue ;
		process_command(read_line, env_copy, exit_code);
		free(read_line);
	}
}

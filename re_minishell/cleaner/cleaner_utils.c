/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 19:54:22 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/29 20:20:00 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

void	cleanup_iteration(char **line, char ***argv,
			t_token **tokens, t_ast **ast)
{
	if (line && *line)
	{
		free(*line);
		*line = NULL;
	}
	if (argv && *argv)
	{
		free_argv(*argv);
		*argv = NULL;
	}
	if (tokens && *tokens)
	{
		free_tokens(*tokens);
		*tokens = NULL;
	}
	if (ast && *ast)
	{
		free_ast(*ast);
		*ast = NULL;
	}
}

void	close_fds(void)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 19:47:50 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/29 20:05:00 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	executable_code(char *code, char *path)
{
	if (ft_strncmp("F_OK", code, 4) == 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		return (ft_putstr_fd(": No such file or directory\n", 2), 127);
	}
	else if (ft_strncmp("X_OK", code, 4) == 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
	}
	else if (ft_strncmp("S_ISDIR", code, 8) == 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
	}
	else if (ft_strncmp("S_ISREG", code, 8))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": not a regular file\n", 2);
	}
	return (126);
}

int	is_executable_path(char *path)
{
	struct stat	sb;

	if (access(path, F_OK) != 0)
		return (executable_code("F_OK", path));
	if (stat(path, &sb) != 0)
		return (perror("minishell: stat"), 126);
	if (S_ISDIR(sb.st_mode))
		return (executable_code("S_ISDIR", path));
	if (!S_ISREG(sb.st_mode))
		return (executable_code("S_ISREG", path));
	if (access(path, X_OK) != 0)
		return (executable_code("X_OK", path));
	return (0);
}

char	*dir_and_cmd_combo_path(char *dir, char *cmd)
{
	char	*result;
	int		dir_len;
	int		cmd_len;

	if (!dir || !cmd)
		return (NULL);
	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	result = malloc(dir_len + cmd_len + 2);
	if (!result)
		return (NULL);
	ft_strcpy(result, dir);
	result[dir_len] = '/';
	ft_strcpy(result + dir_len + 1, cmd);
	return (result);
}

char	*find_cmd_in_path(char *cmd)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	i = 0;
	while (paths[i])
	{
		full_path = dir_and_cmd_combo_path(paths[i], cmd);
		if (access(full_path, X_OK) == 0)
		{
			free_argv(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_argv(paths);
	return (NULL);
}

char	*validate_cmd(char *cmd)
{
	if (!cmd || cmd[0] == '\0')
		return (NULL);
	if (ft_strncmp(cmd, ".", 2) == 0 || ft_strncmp(cmd, "..", 3) == 0)
		return (NULL);
	if (is_builtin(cmd))
		return (ft_strdup(cmd));
	if (cmd[0] == '/' || ft_strncmp(cmd, "./", 2) == 0
		|| ft_strncmp(cmd, "../", 3) == 0)
	{
		if (is_executable_path(cmd) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (find_cmd_in_path(cmd));
}

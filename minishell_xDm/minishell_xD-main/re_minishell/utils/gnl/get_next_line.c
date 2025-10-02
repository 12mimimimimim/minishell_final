/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: didris <didris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 18:53:34 by didris            #+#    #+#             */
/*   Updated: 2025/01/19 18:53:34 by didris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*process_line(char **stash, char **line)
{
	size_t	i;
	char	*temp;

	i = 0;
	while ((*stash)[i] && ((*stash)[i] != '\n'))
		i++;
	*line = ft_substr(*stash, 0, i + 1);
	if (!*line)
		return (NULL);
	if ((*stash)[i] != '\0')
	{
		temp = ft_substr(*stash, i + 1, ft_strlen(*stash) - (i + 1));
		if (!temp)
			return (free(*line), NULL);
		free(*stash);
		*stash = temp;
	}
	else
	{
		free(*stash);
		*stash = NULL;
	}
	return (*line);
}

static char	*read_from_fd(int fd)
{
	char	*lineread;
	ssize_t	bytesread;

	lineread = (char *)malloc(sizeof(char) * ((size_t)BUFFER_SIZE + 1));
	if (!lineread)
		return (NULL);
	bytesread = read(fd, lineread, BUFFER_SIZE);
	if (bytesread <= 0)
	{
		free(lineread);
		return (NULL);
	}
	lineread[bytesread] = '\0';
	return (lineread);
}

static char	*read_from_stash(int fd, char **stash)
{
	char		*lineread;
	char		*temp;

	lineread = read_from_fd(fd);
	while (lineread)
	{
		temp = ft_strjoin(*stash, lineread);
		free(lineread);
		if (!temp)
		{
			free(*stash);
			*stash = NULL;
			return (NULL);
		}
		free(*stash);
		*stash = temp;
		if (ft_strchr(*stash, '\n'))
			break ;
		lineread = read_from_fd(fd);
	}
	return (*stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
		return (free(stash), stash = NULL, NULL);
	if (!stash)
		stash = ft_strdup("");
	stash = read_from_stash(fd, &stash);
	if (!stash)
		return (NULL);
	if ((stash)[0] == '\0')
		return (free(stash), stash = NULL, NULL);
	line = process_line(&stash, &line);
	if (!line)
	{
		free(stash);
		stash = NULL;
	}
	return (line);
}
